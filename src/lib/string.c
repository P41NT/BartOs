#include "include/string.h"

size_t strlen(const char *s) {
    size_t count = 0;
    char* temp = s;
    while (*temp != '\0') {
        temp++;
        count++;
    }
    return count;
}

bool strcmp(const char* s1, const char* s2) {
    char* c1 = s1;
    char* c2 = s2;

    while (*c1 != '\0' || *c2 != '\0') {
        if (*c1 != *c2) return false;
        c1++;
        c2++;
    }
    return true;
}

char* strcpy(char* dst, const char* src) {
    char* s = src;
    char* d = dst;
    while (*s != '\0') {
        *d = *s;
        d++;
        s++;
    }
}

char* strncpy(char* dst, const char* src, size_t n) {
    char *s = src;
    char *d = dst;
    while (n--) {
        *d = *s;
        d++;
        s++;
    }
}

void* memcpy(void* dst, const void* src, size_t n) {
    char *s = (char*)src;
    char *d = (char*)dst;
    while (n--) {
        *d = *s;
        d++;
        s++;
    }
}

void* memset(void* dst, int c, size_t n) {
    char *d = (char*)dst;
    char f = (char*)c;
    while (n--) {
        *d = f;
        d++;
    }
}

bool memcmp(const void* src1, const void* src2, size_t n) {
    char* c1 = s1;
    char* c2 = s2;

    while (n--) {
        if (*c1 != *c2) return false;
        c1++;
        c2++;
    }
    return true;
}

bool isdigit(char c) {
    return (c >= '0' && c <= '9');
}

bool isalpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int atoi(const char* s) {
    int result = 0;
    char *c = s;
    while (*c != '\0') {
        c++;
    }
    c--;
    int exp = 1;
    while (c != s) {
        int dig = (*c) - '0';
        result += dig * exp;
        exp *= 10;
        c--;
    }
    return result;
}
