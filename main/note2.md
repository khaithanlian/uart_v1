#include <stdio.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/uart.h"
#include "driver/i2c_master.h"

#define UART_PORT       UART_NUM_1
#define UART_TX_PIN     17
#define UART_RX_PIN     16
#define RX_BUFFER_SIZE  1024
#define TX_BUFFER_SIZE  1024

// OLED I2C
#define I2C_SDA_PIN     21
#define I2C_SCL_PIN     22
#define OLED_ADDRESS    0x3C

#define OLED_WIDTH      128
#define OLED_HEIGHT     64

static i2c_master_bus_handle_t i2c_bus;
static i2c_master_dev_handle_t oled_dev;

static void oled_command(uint8_t command)
{
    uint8_t data[2];

    data[0] = 0x00;
    data[1] = command;

    ESP_ERROR_CHECK(
        i2c_master_transmit(oled_dev, data, 2, -1)
    );
}

static void oled_data(uint8_t *data, size_t length)
{
    uint8_t buffer[129];

    buffer[0] = 0x40;

    if (length > 128)
        length = 128;

    memcpy(&buffer[1], data, length);

    ESP_ERROR_CHECK(
        i2c_master_transmit(oled_dev, buffer, length + 1, -1)
    );
}

static void oled_init(void)
{
    i2c_master_bus_config_t bus_config = {
        .i2c_port = I2C_NUM_0,
        .sda_io_num = I2C_SDA_PIN,
        .scl_io_num = I2C_SCL_PIN,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
    };

    ESP_ERROR_CHECK(
        i2c_new_master_bus(&bus_config, &i2c_bus)
    );

    i2c_device_config_t dev_config = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = OLED_ADDRESS,
        .scl_speed_hz = 400000,
    };

    ESP_ERROR_CHECK(
        i2c_master_bus_add_device(
            i2c_bus,
            &dev_config,
            &oled_dev
        )
    );

    vTaskDelay(pdMS_TO_TICKS(100));

    oled_command(0xAE);
    oled_command(0xD5);
    oled_command(0x80);
    oled_command(0xA8);
    oled_command(0x3F);
    oled_command(0xD3);
    oled_command(0x00);
    oled_command(0x40);
    oled_command(0x8D);
    oled_command(0x14);
    oled_command(0x20);
    oled_command(0x00);
    oled_command(0xA1);
    oled_command(0xC8);
    oled_command(0xDA);
    oled_command(0x12);
    oled_command(0x81);
    oled_command(0x7F);
    oled_command(0xD9);
    oled_command(0xF1);
    oled_command(0xDB);
    oled_command(0x40);
    oled_command(0xA4);
    oled_command(0xA6);
    oled_command(0xAF);
}

static void oled_clear(void)
{
    uint8_t zeros[128] = {0};

    for (uint8_t page = 0; page < 8; page++)
    {
        oled_command(0xB0 + page);
        oled_command(0x00);
        oled_command(0x10);

        oled_data(zeros, 128);
    }
}

void app_main(void)
{
    // ---------------- UART ----------------

    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };

    ESP_ERROR_CHECK(
        uart_param_config(UART_PORT, &uart_config)
    );

    ESP_ERROR_CHECK(
        uart_set_pin(
            UART_PORT,
            UART_TX_PIN,
            UART_RX_PIN,
            UART_PIN_NO_CHANGE,
            UART_PIN_NO_CHANGE
        )
    );

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

    // ---------------- OLED ----------------

    oled_init();
    oled_clear();

    // ---------------- UART LOOPBACK ----------------

    while (1)
    {
        const char *message = "Hello UART!\r\n";

        // TX: GPIO17
        uart_write_bytes(
            UART_PORT,
            message,
            strlen(message)
        );

        // RX: GPIO16
        uint8_t rx_buffer[128];

        int length = uart_read_bytes(
            UART_PORT,
            rx_buffer,
            sizeof(rx_buffer) - 1,
            pdMS_TO_TICKS(1000)
        );

        if (length > 0)
        {
            rx_buffer[length] = '\0';

            // For now, print received data to console
            printf("Received: %s", rx_buffer);
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}