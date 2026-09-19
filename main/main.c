#include "uart.h"

void app_main(void)
{
    uart_init();

    uart_write("Hello from ESP32!\r\n");
    uart_write("UART transmission is working.\r\n");
}