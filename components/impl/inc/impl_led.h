#ifndef IMPL_LED_H
#define IMPL_LED_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "plat_led.h"

plat_led_dev_t *impl_led_r_dev(void);
plat_led_dev_t *impl_led_g_dev(void);
plat_led_dev_t *impl_led_b_dev(void);

#ifdef __cplusplus
}
#endif

#endif // IMPL_LED_H
