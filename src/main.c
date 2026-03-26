#include "../lib/stm32f401xe.h"
#include "uart.h"

int main() {
    uart_init(115200);
    while (1) {
        uart_write_string("hello there, get to work\n");
        for (volatile int i = 0; i < 1000000; i++);
    }
}
