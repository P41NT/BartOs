#pragma once

void uart_init(unsigned baud_rate);
void uart_write_char(char c);
void uart_write_string(const char *s);
void uart_write_buf(const char *s, int len);
