#include "scheduler.h"
#include "threads.h"
#include "../lib/include/stdlib.h"

volatile int NUM_THREADS;
volatile tcb_t *tcbs;
volatile tcb_t* current_tcb;
volatile int current_task_index;
volatile int current_tid;

volatile int scheduler_started = 0;

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
        "MRS R0, PSP\n"
        "STMDB R0!, {R4-R11}\n"
        "LDR R1, =current_tcb\n"
        "LDR R1, [R1]\n"
        "STR R0, [R1]\n"

        "BL schedule_thread\n"

        "LDR R1, =current_tcb\n" // points to current_tcb
        "LDR R1, [R1]\n"
        "LDR R0, [R1]\n"
        "LDMIA R0!, {R4-R11}\n"
        "MSR PSP, R0\n"
        "BX LR\n"
    );
}

void SysTick_Handler(void) {
    SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
    __DSB();
    __ISB();
}

void idle_task() {
    while (1) {
        __WFI();
    }
}

void scheduler_init(int numThreads, int periodMilliseconds) {
    tcbs = (tcb_t*)bmalloc(numThreads * sizeof(tcb_t));

    NUM_THREADS = numThreads;

    tcbs[IDLE_THREAD].psp = initialize_stack(idle_task, 32);
    tcbs[IDLE_THREAD].state = THREAD_READY;

    current_tid = 1;
    current_tcb = &tcbs[IDLE_THREAD];
    current_task_index = IDLE_THREAD;

    __set_PSP((uint32_t)(current_tcb->psp));

    SysTick_Config(SystemCoreClock / 1000);

    NVIC_SetPriority(PendSV_IRQn, 0xFF);
    NVIC_SetPriority(SysTick_IRQn, 0xFE);

    scheduler_started = 1;

    __set_CONTROL(0x02);
    __ISB();
}
