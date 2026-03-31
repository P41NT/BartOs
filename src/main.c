#include "drivers/uart.h"
#include "kernel/kernel.h"
#include "lib/include/stdio.h"
#include "lib/include/stdlib.h"

void some_hi() {
    while (1) {
        puts("hi");
    }
}

void some_hello() {
    while (1) {
        puts("hi");
    }
}

int main() {
    scheduler_init(5, 20);
    init_allocator();
    uart_init(115200);

    add_thread(some_hi, 20);
    add_thread(some_hello, 20);
}
