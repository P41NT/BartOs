#include "scheduler.h"
#include "threads.h"
#include "../lib/include/stdlib.h"

volatile int NUM_THREADS;
volatile tcb_t *tcbs;
volatile tcb_t* current_tcb;
volatile int current_task_index;
volatile int current_tid;

volatile int scheduler_started = 0;

volatile uint32_t tick_ms = 0;

void delay(int ms) {
    uint32_t start = tick_ms;
    while ((tick_ms - start) < (uint32_t)ms);
}

void schedule_thread() {
    if (current_tcb->state == THREAD_RUNNING && current_task_index != IDLE_THREAD)
        current_tcb->state = THREAD_READY;

    for (int offset = 1; offset < NUM_THREADS; offset++) {
        current_task_index = (current_task_index + 1) % NUM_THREADS;
        if (current_task_index == IDLE_THREAD) continue;
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
        "LDR R0, =scheduler_started\n"
        "LDR R0, [R0]\n"
        "CBZ R0, pendsv_exit\n"

        "MRS R0, PSP\n"
        "STMDB R0!, {R4-R11}\n"
        "LDR R1, =current_tcb\n"
        "LDR R1, [R1]\n"
        "STR R0, [R1]\n"

        "PUSH {LR}\n"
        "BL schedule_thread\n"
        "POP {LR}\n"

        "LDR R1, =current_tcb\n" // points to current_tcb
        "LDR R1, [R1]\n"
        "LDR R0, [R1]\n"
        "LDMIA R0!, {R4-R11}\n"
        "MSR PSP, R0\n"

        "pendsv_exit:\n"
        "BX LR\n"
    );
}

void SysTick_Handler(void) {
    tick_ms++;

    if (scheduler_started) {
        SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
        __DSB();
        __ISB();
    }
}

void idle_task() {
    while (1) {
        __WFI();
    }
}

void scheduler_init(int numThreads, int periodMilliseconds) {
    tcbs = (tcb_t*)bmalloc((numThreads + 1) * sizeof(tcb_t));

    NUM_THREADS = numThreads + 1;

    for (int i = 0; i < NUM_THREADS; i++) {
        tcbs[i].state = THREAD_KILLED;
    }

    tcbs[IDLE_THREAD].psp = initialize_stack(idle_task, 64);
    tcbs[IDLE_THREAD].state = THREAD_READY;

    current_tid = 1;
    current_tcb = &tcbs[IDLE_THREAD];
    current_task_index = IDLE_THREAD;

    NVIC_SetPriority(PendSV_IRQn, 0xFF);
    SysTick_Config(SystemCoreClock / (1000 / periodMilliseconds));
    NVIC_SetPriority(SysTick_IRQn, 0xFE);
}

__attribute__((naked)) void scheduler_start(void) {
    __asm volatile (
        "LDR R0, =current_tcb\n"
        "LDR R0, [R0]\n"
        "LDR R0, [R0]\n"
        "MSR PSP, R0\n"

        "MOV R0, #0x02\n"
        "MSR CONTROL, R0\n"
        "ISB\n"

        "LDR R0, =scheduler_started\n"
        "MOV R1, #1\n"
        "STR R1, [R0]\n"

        "LDR R0, =0xE000ED04\n" // SCB->ICSR
        "LDR R1, =0x10000000\n" // PENDSVSET_Msk
        "STR R1, [R0]\n"
        "ISB\n"

        "scheduler_start_waiting:\n"
        "WFI\n"
        "B scheduler_start_waiting\n"
    );
}


__attribute__((naked)) void HardFault_Handler(void) {
    __asm volatile (
        "TST LR, #4                     \n"
        "ITE EQ                         \n"
        "MRSEQ R0, MSP                  \n"
        "MRSNE R0, PSP                  \n"
        "LDR R1, =hard_fault_handler_c  \n"
        "BX R1                          \n"
    );
}

void hard_fault_handler_c(uint32_t *sp) {
    volatile uint32_t pc   = sp[6];
    volatile uint32_t lr   = sp[5];
    volatile uint32_t xpsr = sp[7];
    (void)pc; (void)lr; (void)xpsr;
    __asm volatile ("BKPT #0");
    while (1);
}
