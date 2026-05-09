#ifndef APP_H
#define APP_H

#include "common_header.h"

#include "impl_exio.h"
#include "impl_spilcd.h"
#include "impl_key.h"
#include "impl_led.h"
#include "impl_wifi.h"

#include "ui.h"

void app_led_task(void *arg);
void app_gui_task(void *arg);
void app_key_task(void *arg);
void app_wifi_task_init(void);

#endif // APP_H
