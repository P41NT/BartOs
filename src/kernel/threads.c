#include "threads.h"
#include "scheduler.h"
#include "kernel.h"

#include "../lib/include/stdlib.h"
#include "../lib/include/stdio.h"

#include <stdint.h>

void spin_task() {
    while (1) {
        __WFI();
    }
}

void thread_end_handler() {
    current_tcb->state = THREAD_KILLED;
    __DSB();
    __ISB();
    while (1) {
        __WFI();
    }
}

uint32_t* initialize_stack(void (*fn_ptr)(void*), int stackSizeWords, void* arguments) {
    uint32_t* stack_ptr = (uint32_t*)bmalloc(stackSizeWords * sizeof(uint32_t));
    uint32_t* sp = &stack_ptr[stackSizeWords];

    *(--sp) = 0x01000000;
    *(--sp) = (uint32_t)fn_ptr; // PC
    *(--sp) = (uint32_t)thread_end_handler; // LR
    // *(--sp) = (uint32_t)spin_task; // LR
    *(--sp) = 0; // R12
    *(--sp) = 0; // R3
    *(--sp) = 0; // R2
    *(--sp) = 0; // R1
    *(--sp) = (uint32_t)arguments; // R0

    *(--sp) = 0; // R11
    *(--sp) = 0; // R10
    *(--sp) = 0; // R9
    *(--sp) = 0; // R8
    *(--sp) = 0; // R7
    *(--sp) = 0; // R6
    *(--sp) = 0; // R5
    *(--sp) = 0; // R4

    return sp;
}

int add_thread(void (*fn_ptr)(void*), int stackSizeWords, void* arguments) {
    uint32_t* stack_ptr = initialize_stack(fn_ptr, stackSizeWords, arguments);

    int tcb_index = 1;
    for (; tcb_index < NUM_THREADS; tcb_index++) {
        if (tcbs[tcb_index].state == THREAD_KILLED) break;
    }

    if (tcb_index == NUM_THREADS) {
        puts("Not enough threads sorry\r\n");
        return -1;
    }

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
