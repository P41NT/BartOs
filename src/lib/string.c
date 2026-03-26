#include "include/string.h"

int strlen(const char *s) {
    int count = 0;
    const char* temp = s;
    while (*temp != '\0') {
        temp++;
        count++;
    }
    return count;
}

int strcmp(const char* s1, const char* s2) {
    const char* c1 = s1;
    const char* c2 = s2;

    while (*c1 != '\0' || *c2 != '\0') {
        if (*c1 != *c2) return 1;
        c1++;
        c2++;
    }
    return 0;
}

char* strcpy(char* dst, const char* src) {
    const char* s = src;
    char* d = dst;
    while (*s != '\0') {
        *d = *s;
        d++;
        s++;
    }
    return dst;
}

char* strncpy(char* dst, const char* src, int n) {
    const char *s = src;
    char *d = dst;
    while (n--) {
        *d = *s;
        d++;
        s++;
    }
    return dst;
}

void* memcpy(void* dst, const void* src, int n) {
    const char *s = (char*)src;
    char *d = (char*)dst;
    while (n--) {
        *d = *s;
        d++;
        s++;
    }
    return dst;
}

void* memset(void* dst, int c, int n) {
    char *d = (char*)dst;
    char f = (char)c;
    while (n--) {
        *d = f;
        d++;
    }
    return dst;
}

int memcmp(const void* src1, const void* src2, int n) {
    const char* c1 = src1;
    const char* c2 = src2;

    while (n--) {
        if (*c1 != *c2) return 1;
        c1++;
        c2++;
    }
    return 0;
}

int isdigit(char c) {
    return (c >= '0' && c <= '9');
}

int isalpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int atoi(const char* s) {
    int result = 0;
    const char *c = s;
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
