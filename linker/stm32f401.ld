ENTRY(Reset_Handler)

MEMORY
{
    FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 512K
    RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 96K
}

_estack = ORIGIN(RAM) + LENGTH(RAM);

SECTIONS
{
    /* Vector table must be first at 0x08000000 */
    .isr_vector :
    {
        . = ALIGN(4);
        KEEP(*(.isr_vector))
        . = ALIGN(4);
    } >FLASH

    /* Code and read-only data */
    .text :
    {
        . = ALIGN(4);
        *(.text.Reset_Handler)  /* force Reset_Handler right after vector table */
        *(.text*)
        *(.rodata*)
        . = ALIGN(4);
        _etext = .;
    } >FLASH

    /* ARM exception unwinding — needed by vendor libs */
    .ARM :
    {
        __exidx_start = .;
        *(.ARM.exidx*)
        __exidx_end = .;
    } >FLASH

    /* Used by libc init */
    .preinit_array :
    {
        PROVIDE_HIDDEN(__preinit_array_start = .);
        KEEP(*(.preinit_array*))
        PROVIDE_HIDDEN(__preinit_array_end = .);
    } >FLASH

    .init_array :
    {
        PROVIDE_HIDDEN(__init_array_start = .);
        KEEP(*(SORT(.init_array.*)))
        KEEP(*(.init_array*))
        PROVIDE_HIDDEN(__init_array_end = .);
    } >FLASH

    .fini_array :
    {
        PROVIDE_HIDDEN(__fini_array_start = .);
        KEEP(*(SORT(.fini_array.*)))
        KEEP(*(.fini_array*))
        PROVIDE_HIDDEN(__fini_array_end = .);
    } >FLASH

    /* Load address of .data in flash */
    _sidata = LOADADDR(.data);

    /* Initialized data — lives in flash, copied to RAM by startup */
    .data :
    {
        . = ALIGN(4);
        _sdata = .;
        *(.data*)
        . = ALIGN(4);
        _edata = .;
    } >RAM AT >FLASH

    /* Uninitialized data — zeroed by startup */
    .bss :
    {
        . = ALIGN(4);
        _sbss = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;
    } >RAM

    /* Heap and stack size checking */
    ._user_heap_stack :
    {
        . = ALIGN(8);
        PROVIDE(end = .);
        . = . + 0x400;  /* 1K minimum heap */
        . = . + 0x400;  /* 1K minimum stack */
        . = ALIGN(8);
    } >RAM

    /* Discard unwanted sections */
    /DISCARD/ :
    {
        libc.a(*)
        libm.a(*)
        libgcc.a(*)
    }
}
