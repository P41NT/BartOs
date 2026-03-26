#include "../../lib/stm32f401xe.h"

#define UART_TX_GPIO 2
#define UART_RX_GPIO 3
#define UART_AF 7
#define LED_GPIO 5

#define PERIPHERAL_CLOCK 16000000

void uart_init(unsigned baud_rate) {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    // Peripheral bus
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    // set GPIO A to enable UART_TX
    GPIOA->MODER &= ~(3 << (UART_TX_GPIO * 2));
    GPIOA->MODER |= (2 << (UART_TX_GPIO * 2));

    // set GPIO A to enable UART_RX
    GPIOA->MODER &= ~(3 << (UART_RX_GPIO * 2));
    GPIOA->MODER |= (2 << (UART_RX_GPIO * 2));

    // set Alternate Function PA3 of UART_RX
    GPIOA->AFR[0] &= ~(0xF << (UART_RX_GPIO * 4));
    GPIOA->AFR[0] |= (UART_AF << (UART_RX_GPIO * 4));

    // set Alternate Function PA2 of UART_TX
    GPIOA->AFR[0] &= ~(0xF << (UART_TX_GPIO * 4));
    GPIOA->AFR[0] |= (UART_AF << (UART_TX_GPIO * 4));

    USART2->BRR = PERIPHERAL_CLOCK / baud_rate;

    // set the word length to 8 bit
    USART2->CR1 &= ~USART_CR1_M;
    USART2->CR1 &= ~USART_CR1_PCE;

    // set 1 stop bit
    USART2->CR2 &= ~(0x3 << (12));

    // enable TE, RE, UE
    USART2->CR1 |= (1 << 3) | (1 << 2);
    USART2->CR1 |= (1 << 13);
}

void uart_write_char(char c) {
    // check state register if something is not written
    while (!(USART2->SR & (1 << 7)));
    USART2->DR = c;
}

void uart_write_string(const char* s) {
    char *c = s;
    while (*c != '\0') {
        uart_write_char(*c);
        c++;
    }
}

void uart_write_buf(const char* s, int len) {
    char *c = s;
    while (len--) {
        uart_write_char(*c);
        c++;
    }
}
