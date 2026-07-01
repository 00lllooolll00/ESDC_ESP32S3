#ifndef BSP_LED_H
#define BSP_LED_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "common_header.h"
#include "bsp_exio.h"

#define BSP_LED_R_PIN BSP_EXIO_NUM0
#define BSP_LED_G_PIN BSP_EXIO_NUM1
#define BSP_LED_B_PIN BSP_EXIO_NUM2

void bsp_led_init(void);
void bsp_led_set(uint8_t value);
void bsp_led_toggle(void);
void bsp_led_r_set(uint8_t value);
void bsp_led_g_set(uint8_t value);
void bsp_led_b_set(uint8_t value);
void bsp_led_r_toggle(void);
void bsp_led_g_toggle(void);
void bsp_led_b_toggle(void);

#ifdef __cplusplus
}
#endif

#endif // LED_H
