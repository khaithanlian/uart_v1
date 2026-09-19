#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"

#define UART_PORT       UART_NUM_1
#define UART_TX_PIN     1
#define UART_RX_PIN     3
#define RX_BUFFER_SIZE  1024
#define TX_BUFFER_SIZE  1024

void app_main(void)
{
    // UART configuration
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };

    // Apply UART configuration
    ESP_ERROR_CHECK(
        uart_param_config(UART_PORT, &uart_config)
    );

    // Configure UART pins
    ESP_ERROR_CHECK(
        uart_set_pin(
            UART_PORT,
            UART_TX_PIN,
            UART_RX_PIN,
            UART_PIN_NO_CHANGE,
            UART_PIN_NO_CHANGE
        )
    );

    // Install UART driver
    ESP_ERROR_CHECK(
        uart_driver_install(
            UART_PORT,
            RX_BUFFER_SIZE,
            TX_BUFFER_SIZE,
            0,
            NULL,
            0
        )
    );

    while (1)
    {
        const char *message = "Hello UART!\r\n";

        uart_write_bytes(
            UART_PORT,
            message,
            13
        );

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}