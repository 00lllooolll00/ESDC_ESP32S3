#ifndef BSP_LED_H
#define BSP_LED_H

#include "driver/gpio.h"
#include "esp_log.h"

#define LED_GPIO_PIN     GPIO_NUM_1

#define BSP_LED_SET(X)   ((X) ? gpio_set_level(LED_GPIO_PIN, 1) : gpio_set_level(LED_GPIO_PIN, 0))
#define BSP_LED_TOGGLE() (gpio_set_level(LED_GPIO_PIN, !gpio_get_level(LED_GPIO_PIN)))

void bsp_led_init(void);

#endif // LED_H
