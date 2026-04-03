#pragma once

void scheduler_init(int numThreads, int periodMilliseconds);
void scheduler_start();

int add_thread(void (*fn_ptr)(void*), int stackSize, void* arguments);
// TODO : Test kill_thread, join_thread, yield_thread
void kill_thread(int tid);
void join_thread(int tid);
void yield_thread(void);

void delay(int ms);

// TODO: Implement Mutex
typedef struct mutex_t {

} mutex_t;

// TODO: Implement Conditional Variables
typedef struct cond_t {

} cond_t;

void lock_mutex(mutex_t* mutex);
void unlock_mutex(mutex_t* mutex);
void cond_wait(cond_t* cond, mutex_t* mutex);
void cond_broadcast(cond_t* cond);

// TODO: Implement Semaphores
typedef struct sem_t {

} sem_t;

void sem_post(sem_t* semaphore);
void sem_wait(sem_t* semaphore);
