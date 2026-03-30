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

extern volatile int NUM_THREADS;
extern volatile tcb_t *tcbs;
extern volatile tcb_t* current_tcb;
extern volatile int current_task_index;
extern volatile int current_tid;
extern volatile int scheduler_started;
