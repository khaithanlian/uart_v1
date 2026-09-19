#include "uart.h"
#include "board.h"
#include "led.h"
#include <string.h>

#define COMMAND_BUFFER_SIZE 64

static void process_command(const char *command)
{
    if (strcmp(command, "LED ON") == 0)
    {
        led_on();
        uart_write("LED is ON\r\n");
    }
    else if (strcmp(command, "LED OFF") == 0)
    {
        led_off();
        uart_write("LED is OFF\r\n");
    }
    else if (strcmp(command, "STATUS") == 0)
    {
        if (led_is_on())
        {
            uart_write("LED STATUS: ON\r\n");
        }
        else
        {
            uart_write("LED STATUS: OFF\r\n");
        }
    }
    else
    {
        uart_write("Unknown command\r\n");
    }
}

static void uart_task(void *pvParameters)
{
    char command[COMMAND_BUFFER_SIZE];
    size_t position = 0;

    while (1)
    {
        char received;

        int count = uart_receive_byte(&received);

        if (count == 1)
        {
            if (received == '\r' || received == '\n')
            {
                command[position] = '\0';

                if (position > 0)
                {
                    process_command(command);
                }

                position = 0;
                continue;
            }

            if (position < COMMAND_BUFFER_SIZE - 1)
            {
                command[position] = received;
                position++;
            }
            else
            {
                position = 0;
                uart_write("Command too long\r\n");
            }
        }
    }

}

void app_main(void)
{
    uart_init();
    led_init();

    uart_write("\r\nUART FreeRTOS Command Interface\r\n");
    uart_write("Commands: LED ON, LED OFF, STATUS\r\n");

    xTaskCreate(
        uart_task,
        "uart_task",
        4096,
        NULL,
        5,
        NULL
    );
}




// uart communication testing
    // uint8_t data[64];

    // while (1)
    // {
    //     int received = uart_read_bytes(
    //         BOARD_UART_PORT,
    //         data,
    //         sizeof(data),
    //         pdMS_TO_TICKS(1000)
    //     );

    //     if (received > 0)
    //     {
    //         uart_write("Start Now!\r\n");

    //         uart_write_bytes(
    //             BOARD_UART_PORT,
    //             data,
    //             received
    //         );

    //         uart_write("...........\r\n");            

    //     }
    // }

    // Writing Uart
    // uart_write("Hello from ESP32!\r\n");
    // uart_write("UART transmission is working.\r\n");