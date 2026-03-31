#include "threads.h"
#include "scheduler.h"
#include "kernel.h"

#include "../lib/include/stdlib.h"

#include <stdint.h>

void spin_task() {
    while (1) {
        __WFI();
    }
}

uint32_t* initialize_stack(void (*fn_ptr)(void), int stackSizeWords) {
    uint32_t* stack_ptr = (uint32_t*)bmalloc(stackSizeWords * sizeof(uint32_t));
    uint32_t* sp = &stack_ptr[stackSizeWords];

    *(--sp) = 0x01000000;
    *(--sp) = (uint32_t)fn_ptr; // PC
    *(--sp) = (uint32_t)spin_task; // LR
    *(--sp) = 0; // R12
    *(--sp) = 0; // R3
    *(--sp) = 0; // R2
    *(--sp) = 0; // R1
    *(--sp) = 0; // R0

    return sp;
}

int add_thread(void (*fn_ptr)(), int stackSizeWords) {
    uint32_t* stack_ptr = initialize_stack(fn_ptr, stackSizeWords);

    int tcb_index = 1;
    for (; tcb_index < NUM_THREADS; tcb_index++) {
        if (tcbs[tcb_index].state == THREAD_KILLED) break;
    }

    if (tcb_index == NUM_THREADS) return -1;

    tcbs[tcb_index].psp = stack_ptr;
    tcbs[tcb_index].state = THREAD_READY;
    tcbs[tcb_index].tid = current_tid++;

    return tcbs[tcb_index].tid;
}

void yield_thread(void) {
    SCB->ICSR = SCB_ICSR_PENDSVSET_Msk;
    __DSB();
    __ISB();
}

void kill_thread(int pid) {
    // TODO: Implement kill_thread(pid)
}
