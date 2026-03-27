#include "scheduler.h"

tcb_t tcbs[NUM_THREADS];
static tcb_t* current_tcb;
static int current_task_index = IDLE_THREAD;
uint32_t idle_stack[IDLE_STACK_SIZE];

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
    for (int offset = 1; offset < NUM_THREADS; offset++) {
        current_task_index = (current_task_index + 1) % NUM_THREADS;
        current_tcb = &tcbs[current_task_index];
        if (current_tcb->state == READY) {
            break;
        }
    }
    current_task_index = IDLE_THREAD;
    current_tcb = &tcbs[current_task_index];
}

void idle_task() {
    while (1) {
        __WFI();
    }
}

void scheduler_init() {
    tcbs[IDLE_THREAD].psp = &idle_stack[IDLE_STACK_SIZE - 16];
    tcbs[IDLE_THREAD].state = READY;

    current_tcb = &tcbs[IDLE_THREAD];
    current_task_index = IDLE_THREAD;

    __set_PSP((uint32_t)(current_tcb->psp));
}

__attribute__((naked)) void PendSV_Handler(void) {
    __asm volatile (
        "BL save_context\n"
        "BL schedule_thread\n"
        "BL restore_context\n"
    );
}
