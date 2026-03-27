#include "scheduler.h"

__attribute__((naked)) void save_context(void) {
    __asm volatile (
        "MRS R0, PSP\n"
        "STMDB R0!, {R4-R11}\n"
        "LDR R1, =current_tcb\n"
        "LDR R1, [R1]\n"
        "STR R0, [R1]\n"
    );
}

__attribute__((naked)) void restore_context(void) {
    __asm volatile (
        "LDR R1, =current_tcb\n" // points to current_tcb
        "LDR R1, [R1]\n"
        "LDR R0, [R1]\n"
        "LDMIA R0!, {R4-R11}\n"
        "MSR PSP, R0\n"
        "BX LR\n"
    );
}

void schedule_thread() {
    if (current_tcb->state == THREAD_RUNNING)
        current_tcb->state = THREAD_WAITING;

    for (int offset = 1; offset < NUM_THREADS; offset++) {
        current_task_index = (current_task_index + 1) % NUM_THREADS;
        current_tcb = &tcbs[current_task_index];

        if (current_tcb->state == THREAD_READY) {
            current_tcb->state = THREAD_RUNNING;
            return;
        }
    }

    current_task_index = IDLE_THREAD;
    current_tcb = &tcbs[current_task_index];
    current_tcb->state = THREAD_RUNNING;
}

__attribute__((naked)) void PendSV_Handler(void) {
    __asm volatile (
        "BL save_context\n"
        "BL schedule_thread\n"
        "BL restore_context\n"
    );
}
