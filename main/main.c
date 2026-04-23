#include "common.h"
#include "app.h"
#include "lvgl.h"

void app_main(void)
{
    app_init();
}

void vApplicationTickHook(void)
{
    lv_tick_inc(1);
}
