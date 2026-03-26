#pragma once

#include "../../drivers/uart.h"

int write(int fd, const char *buf, size_t len);
int read(int fd, char *buf, size_t len);
