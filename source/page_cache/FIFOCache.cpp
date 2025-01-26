#include "FIFOCache.hpp"
#include <cstring>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>

namespace page_cache {

FIFOCache::FIFOCache(size_t capacity) : capacity(capacity) {}

CacheBlock* FIFOCache::getBlock(int fd, off_t offset) {
    off_t block_offset = CalculateBlockOffset(offset);

    auto it = cache.find(block_offset);
    if (it != cache.end()) {
        return &it->second;
    }

    if (cache.size() >= capacity) {
        Evict();
    }

    CacheBlock new_bock;
    new_bock.fd = fd;
    new_bock.offset = block_offset;
    new_bock.data.resize(BLOCK_SIZE, 0);
    new_bock.dirty = false;

    pread(fd, new_bock.data.data(), BLOCK_SIZE, block_offset);
    fifoQueue.push(block_offset);
    cache[block_offset] = std::move(new_bock);

    return &cache[block_offset];
}

void FIFOCache::writeBlock(int fd, off_t offset, const uint8_t* buf, size_t writable_size) {
    CacheBlock* block = getBlock(fd, offset);
    size_t start = offset % BLOCK_SIZE;
    
    std::memcpy(block->data.data() + start, buf, writable_size);
    block->valid_size = std::max(block->valid_size, start + writable_size);
    block->dirty = true;
}

void FIFOCache::sync() {
    for (auto& [offset, block] : cache) {
        if (block.dirty) {
            size_t write_size = block.valid_size;
            if (write_size > 0) {
                if (pwrite(block.fd, block.data.data(), write_size, block.offset) == -1) {
                    perror("pwrite failed");
                }
            }
            block.dirty = false;
        }
    }
}


void FIFOCache::Evict() {
    if (fifoQueue.empty()) {
        return;
    }

    off_t evict_offset = fifoQueue.front();
    fifoQueue.pop();

    auto it = cache.find(evict_offset);
    if (it != cache.end() && it->second.dirty) {
        pwrite(it->second.fd, it->second.data.data(), it->second.data.size(), it->second.offset);
    }
    cache.erase(evict_offset);
}

off_t FIFOCache::CalculateBlockOffset(off_t offset) {
    return (offset / BLOCK_SIZE) * BLOCK_SIZE;
}

} // namespace page_cache
