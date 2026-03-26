#pragma once

void* malloc(size_t size);
void free(void* ptr);

void *calloc(size_t size);
void *realloc(size_t size);

void exit(int status);
void kill(int pid);
int getpid(void);
