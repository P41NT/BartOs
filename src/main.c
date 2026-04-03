#include "drivers/uart.h"
#include "kernel/kernel.h"
#include "lib/include/stdio.h"
#include "lib/include/stdlib.h"

#include "../lib/stm32f401xe.h"
#include "../lib/stm32f4xx.h"
#include <stdint.h>

#define NULL 0

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


struct uart_arg_t {
    char* message;
    int delay_ms;
};

void uarty(void* args) {
    char* message = ((struct uart_arg_t*)args)->message;
    int delay_ms = ((struct uart_arg_t*)args)->delay_ms;

    while (1) {
        printf("hi %s\r\n", message);
        delay(delay_ms);
    }
}

void blinky() {
    led_setup();

    int countBlinks = 0;

    struct uart_arg_t* tempArg = bmalloc(sizeof(struct uart_arg_t));
    char* msg = "hi";
    tempArg->message = msg;
    tempArg->delay_ms = 5000;

    while (1) {
        GPIOA->BSRR = GPIO_BSRR_BS5;   // set PA5
        delay(500);
        GPIOA->BSRR = GPIO_BSRR_BR5;   // reset PA5
        delay(500);

        countBlinks++;

        if (countBlinks == 10) {
            add_thread(uarty, 50, tempArg);
        }
    }
}

void main_thread() {
    uart_init(115200);

    char* message1 = "shawn";
    char* message2 = "moses";
    int delay1 = 1000;
    int delay2 = 2000;

    struct uart_arg_t* arg1 = bmalloc(sizeof(struct uart_arg_t));
    struct uart_arg_t* arg2 = bmalloc(sizeof(struct uart_arg_t));

    arg1->message = message1;
    arg2->message = message2;

    arg1->delay_ms = delay1;
    arg2->delay_ms = delay2;

    add_thread(blinky, 50, NULL);
    add_thread(uarty, 50, arg1);
    add_thread(uarty, 50, arg2);
}

int main() {
    SystemInit();
    SystemCoreClockUpdate();
    SysTick_Config(SystemCoreClock / 1000);

    init_allocator();

    scheduler_init(4, 100);
    add_thread(main_thread, 50, NULL);
    scheduler_start();
}
