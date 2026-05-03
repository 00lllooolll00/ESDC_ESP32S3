#ifndef APP_H
#define APP_H

#include "common_header.h"

#include "impl_exio.h"
#include "impl_spilcd.h"
#include "impl_key.h"
#include "impl_led.h"

#include "app_gui.h"

void app_led_task(void *arg);
void app_gui_task(void *arg);
void app_key_task(void *arg);

#endif // APP_H
