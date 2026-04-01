#include "drivers/uart.h"
#include "kernel/kernel.h"
#include "lib/include/stdio.h"
#include "lib/include/stdlib.h"

#include "../lib/stm32f401xe.h"
#include "../lib/stm32f4xx.h"
#include <stdint.h>

void __libc_init_array(void) {
}

void led_setup() {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    GPIOA->MODER &= ~GPIO_MODER_MODER5;
    GPIOA->MODER |= GPIO_MODER_MODER5_0;

    GPIOA->OTYPER  &= ~GPIO_OTYPER_OT5;
    GPIOA->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED5;
    GPIOA->PUPDR   &= ~GPIO_PUPDR_PUPDR5;
}

void blinky() {
    led_setup();

    while (1) {
        GPIOA->BSRR = GPIO_BSRR_BS5;   // set PA5
        delay(500);
        GPIOA->BSRR = GPIO_BSRR_BR5;   // reset PA5
        delay(500);
    }
}

void uarty() {
    uart_init(115200);

    while (1) {
        puts("some hi\n");
        delay(1000);
    }
}

void uarty2() {
    uart_init(115200);

    while (1) {
        puts("some hello\n");
        delay(3000);
    }
}

void main_thread() {
    add_thread(blinky, 50);
    add_thread(uarty, 50);
    add_thread(uarty2, 50);
}

int main() {
    SystemInit();
    SystemCoreClockUpdate();
    SysTick_Config(SystemCoreClock / 1000);

    init_allocator();

    scheduler_init(4, 100);
    add_thread(main_thread, 50);
    scheduler_start();
}
