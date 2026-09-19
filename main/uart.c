#include "uart.h"
#include "board.h"

#include "driver/uart.h"
#include "esp_err.h"
#include <string.h>

#define UART_BAUD_RATE       115200
#define UART_RX_BUFFER_SIZE  1024
#define UART_TX_BUFFER_SIZE  1024

void uart_init(void)
{
    const uart_config_t config = {
        .baud_rate = UART_BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };

    ESP_ERROR_CHECK(
        uart_driver_install(
            BOARD_UART_PORT,
            UART_RX_BUFFER_SIZE,
            UART_TX_BUFFER_SIZE,
            0,
            NULL,
            0
        )
    );

    ESP_ERROR_CHECK(
        uart_param_config(
            BOARD_UART_PORT,
            &config
        )
    );

    ESP_ERROR_CHECK(
        uart_set_pin(
            BOARD_UART_PORT,
            BOARD_UART_TX_GPIO,
            BOARD_UART_RX_GPIO,
            UART_PIN_NO_CHANGE,
            UART_PIN_NO_CHANGE
        )
    );
}

void uart_write(const char *data)
{
    uart_write_bytes(
        BOARD_UART_PORT,
        data,
        strlen(data)
    );
}