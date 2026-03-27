#pragma once

void scheduler_init(int numThreads, int periodMilliseconds);
int add_thread(void (*fn_ptr)(), int stackSize);
void yield_thread(void);
void kill_thread(int pid);
