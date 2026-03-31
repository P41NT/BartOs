/* TODO: Learn how this works and how to write my own */

.syntax unified
.cpu cortex-m4
.thumb

.global Reset_Handler
.global SysTick_Handler
.global PendSV_Handler
.global HardFault_Handler

_estack = 0x20018000  /* top of RAM from linker script */

.section .isr_vector, "a", %progbits
g_pfnVectors:
    .word _estack
    .word Reset_Handler
    .word 0                 /* NMI */
    .word HardFault_Handler /* HardFault */
    .word 0,0,0,0           /* MemManage, BusFault, UsageFault */
    .word 0                  /* reserved */
    .word 0                  /* reserved */
    .word 0                  /* reserved */
    .word 0                  /* reserved */
    .word 0                  /* SVC */
    .word 0                  /* DebugMon */
    .word 0                  /* reserved */
    .word PendSV_Handler
    .word SysTick_Handler

.section .text
Reset_Handler:
    /* Copy .data from flash to RAM */
    ldr r0, =_sdata
    ldr r1, =_edata
    ldr r2, =_sidata
copy_data:
    cmp r0, r1
    bge zero_bss
    ldr r3, [r2], #4
    str r3, [r0], #4
    b copy_data

zero_bss:
    ldr r0, =_sbss
    ldr r1, =_ebss
    mov r2, #0
zero_loop:
    cmp r0, r1
    bge main_call
    str r2, [r0], #4
    b zero_loop

main_call:
    bl main
hang:
    b hang

/* Minimal default handlers */
.section .text
HardFault_Handler:
    b .
.weak PendSV_Handler
.thumb_set PendSV_Handler, HardFault_Handler

.weak SysTick_Handler
.thumb_set SysTick_Handler, HardFault_Handler
