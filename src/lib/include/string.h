#pragma once

size_t strlen(const char* s);
bool strcmp(const char* s1, const char* s2);
char* strcpy(char* dst, const char* src);
char* strncpy(char* dst, const char* src, size_t n);

void* memcpy(void* dst, const void* src, size_t n);
void* memset(void* dst, int c, size_t n);
bool memcmp(const void* src1, const void* src2, size_t n);

bool isdigit(char c);
bool isalpha(char c);

int atoi(const char* s);
