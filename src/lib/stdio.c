#include "include/stdio.h"
#include <stdarg.h>

void print_int(int x) {
    char reversed[20];
    char* r = reversed;
    while (x) {
        *r = ((x % 10) + '0');
        r++;
        x /= 10;
    }
    while (r--) {
        putchar(*r);
    }
}

int printf(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    const char* curr = fmt;

    while (*curr) {
        if (*curr == '%') {
            curr++;
            char temp[5];
            char* t = temp;
            while (t < temp + 4 && isalpha(*curr)) {
                *t = *curr;
                t++;
                curr++;
            }
            *t = '\0';

            if (!strcmp(temp, "d")) {
                int val = va_arg(args, int);
                print_int(val);
            }
            else if (!strcmp(temp, "s")) {
                char* str = va_arg(args, char*);
                puts(str);
            }
        }
        else {
            putchar(*curr);
            curr++;
        }
    }

    va_end(args);
    return 0;
}

int putchar(char c) {
    uart_write_char(c);
    return 0;
}

int puts(const char* s) {
    uart_write_string(s);
    return 0;
}
