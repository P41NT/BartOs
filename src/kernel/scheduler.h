#pragma once

#include <stdint.h>
#include "../../lib/stm32f401xe.h"

#define NUM_THREADS 4
#define IDLE_THREAD 0
#define IDLE_STACK_SIZE 128

enum program_state_t {
    RUNNING,
    WAITING,
    READY
};

typedef struct tcb {
    uint32_t* psp;
    enum program_state_t state;
} tcb_t;

void scheduler_init();
