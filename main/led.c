#include "led.h"
#include "board.h"

#include "driver/gpio.h"

static int led_state = 0;

void led_init(void)
{
    gpio_config_t config = {
        .pin_bit_mask = 1ULL << BOARD_LED_GPIO,
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };

    gpio_config(&config);

    led_off();
}

void led_on(void)
{
    gpio_set_level(BOARD_LED_GPIO, 1);
    led_state = 1;
}

void led_off(void)
{
    gpio_set_level(BOARD_LED_GPIO, 0);
    led_state = 0;
}

int led_is_on(void)
{
    return led_state;
}