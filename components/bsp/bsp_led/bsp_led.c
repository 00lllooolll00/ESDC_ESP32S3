#include "bsp_led.h"
#include "bsp_exio.h"

FILE_TAG("bsp_led.c");

void bsp_led_init(void)
{
    bsp_exio_pin_config_t led_config = {
        .pin = BSP_LED_R_PIN,
        .mode = BSP_EXIO_PIN_MODE_OUTPUT,
    };

    bsp_exio_conifg_pin(&led_config);
    led_config.pin = BSP_LED_G_PIN;
    bsp_exio_conifg_pin(&led_config);
    led_config.pin = BSP_LED_B_PIN;
    bsp_exio_conifg_pin(&led_config);
    bsp_led_set(1);

    LOG_INFO("bsp led init ok");
}

void bsp_led_set(uint8_t value)
{
    bsp_led_r_set(value);
    bsp_led_g_set(value);
    bsp_led_b_set(value);
}

void bsp_led_toggle(void)
{
    uint8_t value = !bsp_exio_read_pin(BSP_LED_R_PIN);
    bsp_led_set(value);
}

void bsp_led_r_set(uint8_t value)
{
    bsp_exio_write_pin(BSP_LED_R_PIN, value ? 1 : 0);
}

void bsp_led_g_set(uint8_t value)
{
    bsp_exio_write_pin(BSP_LED_G_PIN, value ? 1 : 0);
}

void bsp_led_b_set(uint8_t value)
{
    bsp_exio_write_pin(BSP_LED_B_PIN, value ? 1 : 0);
}

void bsp_led_r_toggle(void)
{
    bsp_led_r_set(!bsp_exio_read_pin(BSP_LED_R_PIN));
}

void bsp_led_g_toggle(void)
{
    bsp_led_g_set(!bsp_exio_read_pin(BSP_LED_G_PIN));
}

void bsp_led_b_toggle(void)
{
    bsp_led_b_set(!bsp_exio_read_pin(BSP_LED_B_PIN));
}
