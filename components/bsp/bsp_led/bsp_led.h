#ifndef BSP_LED_H
#define BSP_LED_H

#include "driver/gpio.h"
#include "esp_log.h"

#define BSP_LED_GPIO_PIN GPIO_NUM_1

void bsp_led_init(void);
void bsp_led_set(uint8_t value);
void bsp_led_toggle(void);

#endif // LED_H
