.syntax unified
.cpu cortex-m4
.thumb

.global _estack
.global Reset_Handler
.global SysTick_Handler
.global PendSV_Handler

_estack = 0x20018000

/* Vector table */
.section .isr_vector, "a", %progbits
g_pfnVectors:
    .word _estack
    .word Reset_Handler

    .word 0 /* NMI */
    .word 0 /* HardFault */
    .word 0 /* MemManage */
    .word 0 /* BusFault */
    .word 0 /* UsageFault */
    .word 0,0,0,0
    .word 0 /* SVC */
    .word 0 /* DebugMon */
    .word 0
    .word PendSV_Handler
    .word SysTick_Handler

/* Reset handler */
.section .text.Reset_Handler
Reset_Handler:
    bl main
    b .
