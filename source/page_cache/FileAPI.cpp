#include "FileAPI.hpp"

#include <fcntl.h>
#include <unistd.h>
#include <cstring>

#include "FIFOCache.hpp"

#define CACHE_SIZE 256
#define FILE_PERMISSIONS 0644

static page_cache::FIFOCache cache(CACHE_SIZE);

namespace page_cache {

int lab2_open(const char *path) {
    return open(path, O_RDWR | O_CREAT | O_DIRECT, FILE_PERMISSIONS);
}

int lab2_close(int fd) {
    cache.sync();
    return close(fd);
}

ssize_t lab2_read(int fd, void* buf, size_t count) {
    off_t offset = lseek(fd, 0, SEEK_CUR);
    size_t remaining = count;
    uint8_t* buf_ptr = static_cast<uint8_t*>(buf);

    size_t bytes_read = 0;

    while (remaining > 0) {
        CacheBlock* block = cache.getBlock(fd, offset);
        if (!block) return -1;

        size_t start = offset % BLOCK_SIZE;
        size_t readable_size = std::min(remaining, BLOCK_SIZE - start);

        std::memcpy(buf_ptr, block->data.data() + start, readable_size);

        buf_ptr += readable_size;
        offset += readable_size;
        remaining -= readable_size;
        bytes_read += readable_size;
    }

    lseek(fd, bytes_read, SEEK_CUR);
    return bytes_read;
}

ssize_t lab2_write(int fd, const void* buf, size_t count) {
    off_t offset = lseek(fd, 0, SEEK_CUR);
    size_t remaining = count;
    const uint8_t* buf_ptr = static_cast<const uint8_t*>(buf);

    while (remaining > 0) {
        size_t writable_size = remaining;
        if (remaining > BLOCK_SIZE - (offset % BLOCK_SIZE)) 
            writable_size = BLOCK_SIZE - (offset % BLOCK_SIZE);
        cache.writeBlock(fd, offset, buf_ptr, writable_size);
        offset += writable_size;
        buf_ptr += writable_size;
        remaining -= writable_size;
    }

    lseek(fd, count, SEEK_CUR);
    return count;
}


off_t lab2_lseek(int fd, off_t offset, int whence) {
    return lseek(fd, offset, whence);
}

int lab2_fsync(int fd) {
    cache.sync();
    return fsync(fd);
}

} // namespace page_cache


