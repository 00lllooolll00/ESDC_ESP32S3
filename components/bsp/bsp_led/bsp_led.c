#include "bsp_led.h"

static const char *TAG = "led.c";

void bsp_led_init(void)
{
    gpio_config_t led_config = {
        .pin_bit_mask = 1ull << 1,
        .mode = GPIO_MODE_INPUT_OUTPUT,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };

    esp_err_t err = gpio_config(&led_config);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "leds failed to initialize");
        while (1);
    }

    BSP_LED_SET(1);
}
