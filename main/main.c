#include "common.h"
#include "app.h"
#include "lvgl.h"
#include "ek_export.h"

static const char *TAG = "main.c";

void app_main(void)
{
    ek_export_init();
    // app_init();

    while (1)
    {
        vTaskDelay(100);
    }
}

void vApplicationTickHook(void)
{
    lv_tick_inc(1);
}
