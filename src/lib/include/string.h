#pragma once

int strlen(const char* s);
int strcmp(const char* s1, const char* s2);
char* strcpy(char* dst, const char* src);
char* strncpy(char* dst, const char* src, int n);

void* memcpy(void* dst, const void* src, int n);
void* memset(void* dst, int c, int n);
int memcmp(const void* src1, const void* src2, int n);

int isdigit(char c);
int isalpha(char c);

int atoi(const char* s);
