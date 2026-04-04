#include "../../lib/stm32f401xe.h"

#include "../lib/include/string.h"
#include <stdint.h>

#define UART_TX_GPIO 2
#define UART_RX_GPIO 3
#define UART_AF 7
#define LED_GPIO 5

#define PERIPHERAL_CLOCK 16000000

void uart_init(unsigned baud_rate) {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    GPIOA->MODER &= ~(GPIO_MODER_MODER2 | GPIO_MODER_MODER3);
    GPIOA->MODER |= (GPIO_MODER_MODER2_1 | GPIO_MODER_MODER3_1);

    GPIOA->AFR[0] |= (0x07 << GPIO_AFRL_AFSEL2_Pos) | (0x07 << GPIO_AFRL_AFSEL3_Pos);

    USART2->BRR = PERIPHERAL_CLOCK / baud_rate;
    USART2->CR1 = USART_CR1_TE_Msk; // Will take care of recieving later..
    USART2->CR1 |= USART_CR1_UE_Msk;

    USART2->CR3 |= USART_CR3_DMAT_Msk;
}

void uart_write_char(char c) {
    // check state register if something is not written
    while (!(USART2->SR & (1 << 7)));
    USART2->DR = c;
}

void uart_write_string(const char* s) {
    int len = strlen(s);

    while (DMA1_Stream6->CR & DMA_SxCR_EN);

    DMA1_Stream6->CR = 0;
    DMA1->HIFCR |= (0x3f << 16);
    USART2->SR &= ~USART_SR_TC;

    DMA1_Stream6->M0AR = (uint32_t)s;
    DMA1_Stream6->NDTR = len;
    DMA1_Stream6->PAR = (uint32_t)&(USART2->DR);

    DMA1_Stream6->CR = (4 << DMA_SxCR_CHSEL_Pos) | DMA_SxCR_MINC_Msk | DMA_SxCR_DIR_0 | DMA_SxCR_TCIE_Msk;

    DMA1_Stream6->CR |= DMA_SxCR_EN_Msk;
}

void uart_write_buf(const char* s, int len) {
    const char *c = s;
    while (len--) {
        uart_write_char(*c);
        c++;
    }
}
