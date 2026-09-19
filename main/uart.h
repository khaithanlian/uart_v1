#ifndef UART_H
#define UART_H

#include <stddef.h>

void uart_init(void);
int uart_receive_byte(char *data);
void uart_write(const char *data);

#endif