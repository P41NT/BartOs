#pragma once

#include "../../drivers/uart.h"

int write(int fd, const char *buf, int len);
int read(int fd, char *buf, int len);
