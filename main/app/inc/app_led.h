#ifndef APP_LED_H
#define APP_LED_H

#include "common_header.h"

extern TaskHandle_t g_led_handle;
void app_led_init(void);

#endif // APP_LED_H
