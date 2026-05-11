#ifndef IMPL_LED_H
#define IMPL_LED_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "plat_led.h"

int impl_led_register(plat_led_dev_t *led_dev);

#ifdef __cplusplus
}
#endif

#endif // IMPL_LED_H