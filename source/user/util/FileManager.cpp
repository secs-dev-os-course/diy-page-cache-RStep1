#include "FileManager.hpp"

#include <dlfcn.h>
#include <cstring>
#include <unistd.h>
#include <iostream>
#include <fstream>


namespace user::util {

namespace page_cache {
    extern "C" {
        int lab2_open(const char* path);
        int lab2_close(int fd);
        ssize_t lab2_read(int fd, void* buf, size_t count);
        ssize_t lab2_write(int fd, const void* buf, size_t count);
        off_t lab2_lseek(int fd, off_t offset, int whence);
        int lab2_fsync(int fd);
    }
}

#pragma clang optimize off

void FileManager::WriteLine(const std::string& filename, const std::string& content, bool append) {
    void* handler = dlopen("./build/source/libpage_cache.so", RTLD_LAZY);
    if (!handler) {
        std::cerr << "Failed to load library: " << dlerror() << '\n';
        throw std::ios_base::failure("Error opening library");
    }

    auto lab2_open = (int (*)(const char*))dlsym(handler, "lab2_open");
    auto lab2_close = (int (*)(int))dlsym(handler, "lab2_close");
    auto lab2_write = (ssize_t (*)(int, const void*, size_t))dlsym(handler, "lab2_write");
    auto lab2_lseek = (off_t (*)(int, off_t, int))dlsym(handler, "lab2_lseek");

    if (!lab2_open || !lab2_close || !lab2_write || !lab2_lseek) {
        std::cerr << "Failed to load FIFO Cache function: " << dlerror() << '\n';
        dlclose(handler);
        throw std::ios_base::failure("Error loading functions from library");
    }

    int fd = lab2_open(filename.c_str());
    if (fd < 0) {
        std::cerr << "Failed to open file\n";
        dlclose(handler);
        throw std::ios_base::failure("Error opening file");
    }

    if (append) {
        lab2_lseek(fd, 0, SEEK_END);
    } else {
        lab2_lseek(fd, 0, SEEK_SET);
    }

    ssize_t bytes_written = lab2_write(fd, content.c_str(), content.size());
    if (bytes_written < 0) {
        std::cerr << "Failed to write to file\n";
        lab2_close(fd);
        dlclose(handler);
        throw std::ios_base::failure("Error writing to file");
    }

    lab2_close(fd);
    dlclose(handler);
}

void FileManager::ClearFile(const std::string& filename) {
  std::ofstream out_file(filename, std::ios::out | std::ios::trunc);
  if (!out_file) {
    throw std::ios_base::failure("Error clearing file: " + filename);
  }
}

#pragma clang optimize on

}  // namespace user::util
