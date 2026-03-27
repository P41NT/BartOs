#include "scheduler.h"
#include "threads.h"
#include "../lib/include/stdlib.h"

#include "kernel.h"

void idle_task() {
    while (1) {
        __WFI();
    }
}

void scheduler_init(int numThreads, int periodMilliseconds) {
    tcbs = (tcb_t*)bmalloc(numThreads * sizeof(tcb_t));

    tcbs[IDLE_THREAD].psp = initialize_stack(idle_task, 32);
    tcbs[IDLE_THREAD].state = THREAD_READY;

    current_tid = 1;
    current_tcb = &tcbs[IDLE_THREAD];
    current_task_index = IDLE_THREAD;

    __set_PSP((uint32_t)(current_tcb->psp));

    NVIC_SetPriority(PendSV_IRQn, 0xFF);
    __set_CONTROL(0x02);
    __ISB();
}
