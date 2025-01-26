#pragma once
#ifdef _WIN32
#define HIDDEN_API
#else
#define HIDDEN_API __attribute__((visibility("hidden")))
#endif

#include <fcntl.h>
#include <cstdint>
#include <queue>
#include <unordered_map>
#include <vector>

#define BLOCK_SIZE 4096

namespace page_cache {

struct CacheBlock {
    int fd;
    off_t offset;
    std::vector<uint8_t> data;
    bool dirty;
    size_t valid_size;
};

class FIFOCache {
public:
    FIFOCache(size_t capacity);
    ~FIFOCache() = default;

    CacheBlock* getBlock(int fd, off_t offset);
    void writeBlock(int fd, off_t offset, const uint8_t* buf, size_t writable_size);
    void sync();

private:
    size_t capacity;
    std::queue<off_t> fifoQueue;
    std::unordered_map<off_t, CacheBlock> cache;

    void Evict();
    off_t CalculateBlockOffset(off_t offset);
};

} // namespace page_cache
