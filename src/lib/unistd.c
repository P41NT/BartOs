#include "lib/unistd.h"

int write(int fd, const char *buf, size_t len) {
    if (fd != 0) return -1;
    uart_write_buf(buf, len);
}

int read(int fd, char *buf, size_t len) {
    // TODO: Write something
    return -1;
}
