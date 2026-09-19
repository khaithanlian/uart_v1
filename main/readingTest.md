#include "uart.h"

#include <stdio.h>

void app_main(void)
{
    uart_init();

    uart_write("\r\nUART RX TEST\r\n");
    uart_write("Type something and press Enter:\r\n");

    while (1)
    {
        char received;

        int count = uart_receive_byte(&received);

        if (count == 1)
        {
            char message[64];

            snprintf(
                message,
                sizeof(message),
                "Received: '%c'  HEX: 0x%02X\r\n",
                received,
                (unsigned char)received
            );

            uart_write(message);
        }
    }
}