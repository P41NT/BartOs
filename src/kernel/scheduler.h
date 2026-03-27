#pragma once

#include <stdint.h>
#include "../../lib/stm32f401xe.h"

#define IDLE_THREAD 0

enum program_state_t {
    THREAD_RUNNING,
    THREAD_WAITING,
    THREAD_READY,
    THREAD_KILLED
};

typedef struct tcb {
    uint32_t* psp;
    enum program_state_t state;
    uint32_t tid;
} tcb_t;

extern int NUM_THREADS;
extern tcb_t *tcbs;
static tcb_t* current_tcb;
static int current_task_index = IDLE_THREAD;
static int current_tid;
