#include <iostream>
#include <dlfcn.h>
#include <cstring>
#include <unistd.h>

namespace user::app {

namespace {

int Main() {
    using lab2_open_t = int(const char*);
    using lab2_close_t = int(int);
    using lab2_read_t = ssize_t(int, void*, size_t);
    using lab2_write_t = ssize_t(int, const void*, size_t);
    using lab2_lseek_t = off_t(int, off_t, int);
    using lab2_fsync_t = int(int);

    void *handler = dlopen("./build/source/libpage_cache.so", RTLD_LAZY);
    if (!handler) {
        std::cerr << "Failed to load library: " << dlerror() << '\n';
        return 1;
    }

    std::cout << "dlopen\n";

    auto lab2_open = (lab2_open_t*)dlsym(handler, "lab2_open");
    auto lab2_close = (lab2_close_t*)dlsym(handler, "lab2_close");
    auto lab2_read = (lab2_read_t*)dlsym(handler, "lab2_read");
    auto lab2_write = (lab2_write_t*)dlsym(handler, "lab2_write");
    auto lab2_lseek = (lab2_lseek_t*)dlsym(handler, "lab2_lseek");
    auto lab2_fsync = (lab2_fsync_t*)dlsym(handler, "lab2_fsync");

    if (!lab2_open || !lab2_close || !lab2_read || !lab2_write || !lab2_lseek || !lab2_fsync) {
        std::cerr << "Failed to load FIFO Cache function: " << dlerror() << '\n';
        dlclose(handler);
        return 1;
    }

    int fd = lab2_open("./data/file");
    if (fd < 0) {
        std::cerr << "Failed to open file\n";
        dlclose(handler);
        return 1;
    }

    const char* write_data = "Hello, this is a test write!\n";
    size_t write_size = std::strlen(write_data);

    ssize_t bytes_written = lab2_write(fd, write_data, write_size);
    if (bytes_written < 0) {
        std::cerr << "Failed to write to file\n";
    } else {
        std::cout << "Successfully wrote " << bytes_written << " bytes to file.\n";
    }
    // std::cout << "W"

    off_t new_offset = lab2_lseek(fd, 0, SEEK_SET);
    if (new_offset < 0) {
        std::cerr << "Failed to seek in file\n";
    } else {
        std::cout << "Successfully moved file pointer to offset " << new_offset << ".\n";
    }

    char read_buffer[128] = {0};
    ssize_t bytes_read = lab2_read(fd, read_buffer, sizeof(read_buffer) - 1);
    if (bytes_read < 0) {
        std::cerr << "Failed to read from file\n";
    } else {
        // read_buffer[bytes_read] = '\0';
        std::cout << "Successfully read " << bytes_read << " bytes: " << read_buffer << "\n";
    }

    if (lab2_fsync(fd) < 0) {
        std::cerr << "Failed to sync file\n";
    } else {
        std::cout << "Successfully synced file to disk.\n";
    }

    if (lab2_close(fd) < 0) {
        std::cerr << "Failed to close file\n";
    } else {
        std::cout << "Successfully closed file.\n";
    }

    dlclose(handler);

    std::cout << "Hello from user app\n";
    return 0;
}

} // namespace

} // namespace user::app

int main() {
    return user::app::Main();
}
