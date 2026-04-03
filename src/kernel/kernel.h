#pragma once

void scheduler_init(int numThreads, int periodMilliseconds);
void scheduler_start();
int add_thread(void (*fn_ptr)(void*), int stackSize, void* arguments);
void yield_thread(void);
void kill_thread(int pid);

void delay(int ms);
