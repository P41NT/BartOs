#pragma once

void init_allocator();

void* bmalloc(int size);
void bfree(void* ptr);

void *calloc(int size);
void *realloc(int size);
