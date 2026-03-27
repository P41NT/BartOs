#include "threads.h"
#include "scheduler.h"
#include "kernel.h"

#include "../lib/include/stdlib.h"

#include <stdint.h>

uint32_t* initialize_stack(void (*fn_ptr)(void), int stack_size) {
    uint32_t* stack_ptr = (uint32_t*)bmalloc(stack_size);
    // TODO: initialize context into stack from fn_ptr
    return stack_ptr;
}

int add_thread(void (*fn_ptr)(), int stackSize) {
    uint32_t* stack_ptr = initialize_stack(fn_ptr, stackSize);

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
    __DSB();
    __ISB();
    SCB->ICSR = SCB_ICSR_PENDSVSET_Msk;
}

void kill_thread(int pid) {

}
