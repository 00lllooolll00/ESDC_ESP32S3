#include "common.h"
#include "app.h"
#include "lvgl.h"

static const char *TAG = "main.c";

void app_main(void)
{
    app_init();

    while (1)
    {
        vTaskDelay(100);
    }
}

void vApplicationTickHook(void)
{
    lv_tick_inc(1);
}
