#pragma once

void* malloc(int size);
void free(void* ptr);

void *calloc(int size);
void *realloc(int size);

void exit(int status);
void kill(int pid);
int getpid(void);
