#include "include/unistd.h"

int write(int fd, const char *buf, int len) {
    if (fd != 0) return -1;
    uart_write_buf(buf, len);
    return 0;
}

int read(int fd, char *buf, int len) {
    // TODO: Write something
    return -1;
}
