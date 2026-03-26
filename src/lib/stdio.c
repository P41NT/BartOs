#include "include/stdio.h"
#include "include/string.h"

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
    va_start(args);

    char* curr = fmt;
    char temp[5];
    temp[4] = '\0';

    while (...) {
        while (*curr != '\0' && *curr != '%') curr++;
        if (curr == '\0') return 0;
        curr++;

        char* t = temp;
        while (t < temp + 5 && isalpha(*curr)) {
            *t = *curr;
            t++;
            curr++;
        }

        if (!strcmp(temp, "d")) {
            print_int(va_args(args, int));
        }
        else if (!strcmp(temp, "s")) {
            puts(va_args(args, char*));
        }
    }
}

int putchar(char c) {
    uart_write_char(c);
}

int puts(const char* s) {
    uart_write_string(s);
}
