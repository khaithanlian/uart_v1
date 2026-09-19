#ifndef BOARD_H
#define BOARD_H

#include "driver/uart.h"
#include "driver/gpio.h"

#define BOARD_UART_PORT       UART_NUM_1
#define BOARD_UART_TX_GPIO    1
#define BOARD_UART_RX_GPIO    3

#define BOARD_LED_GPIO        GPIO_NUM_2 // BLUE LED ON ESP BOARD
#endif
