#include "bsp_key.h"

FILE_TAG("bsp_key.c");

void bsp_key_init(void)
{
    gpio_config_t boot_init_config = {
        .pin_bit_mask = (1ull << BSP_KEY_BOOT_PIN),
        .mode = GPIO_MODE_INPUT,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };

    ESP_ERROR_CHECK(gpio_config(&boot_init_config));

    bsp_exio_pin_config_t exio_key_config = {
        .pin = BSP_KEY0_PIN,
        .mode = BSP_EXIO_PIN_MODE_INPUT,
    };

    bsp_exio_conifg_pin(&exio_key_config);
    exio_key_config.pin = BSP_KEY1_PIN;
    bsp_exio_conifg_pin(&exio_key_config);
    exio_key_config.pin = BSP_KEY2_PIN;
    bsp_exio_conifg_pin(&exio_key_config);

    LOG_INFO("bsp key init ok");
}

bsp_key_state_t bsp_key_read_raw(void)
{
    bsp_key_state_t temp = BSP_KEY_STATE_NONE;

    if (!gpio_get_level(BSP_KEY_BOOT_PIN)) temp |= BSP_KEY_STATE_BOOT;
    if (!bsp_exio_read_pin(BSP_KEY0_PIN)) temp |= BSP_KEY_STATE_0;
    if (!bsp_exio_read_pin(BSP_KEY1_PIN)) temp |= BSP_KEY_STATE_1;
    if (!bsp_exio_read_pin(BSP_KEY2_PIN)) temp |= BSP_KEY_STATE_2;

    return temp;
}
