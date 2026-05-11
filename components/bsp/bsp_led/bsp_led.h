#ifndef BSP_LED_H
#define BSP_LED_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "common_header.h"

#define BSP_LED_GPIO_PIN GPIO_NUM_1

void bsp_led_init(void);
void bsp_led_set(uint8_t value);
void bsp_led_toggle(void);

#ifdef __cplusplus
}
#endif

#endif // LED_H
