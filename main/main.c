#include "common.h"
#include "app.h"
#include "lvgl.h"
#include "ek_export.h"

void app_main(void)
{
    ek_export_init();
    app_init();

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void vApplicationTickHook(void)
{
    lv_tick_inc(1);
}
