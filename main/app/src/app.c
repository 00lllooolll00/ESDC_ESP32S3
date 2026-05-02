#include "app.h"
#include "lvgl.h"

FILE_TAG("app.c");

extern plat_lcd_dev_t g_lcd_dev;
extern plat_key_dev_t g_key_dev;
extern plat_led_dev_t g_led_dev;

void app_led_task(void *arg)
{
    while (1)
    {
        plat_led_dev_toggle(&g_led_dev);
        vTaskDelay(500);
    }
}

void app_gui_task(void *arg)
{
    app_gui_init();
    while (1)
    {
        uint32_t delay = lv_task_handler();
        vTaskDelay(delay >= 5 ? delay : 5);
    }
}

void app_key_task(void *arg)
{
    plat_key_state_t key_old = PLAT_KEY_STATE_NONE;
    TickType_t start_tick = xTaskGetTickCount();
    while (1)
    {
        plat_key_state_t key_temp;
        plat_key_dev_read_raw(&g_key_dev, &key_temp);
        plat_key_state_t key_val = key_temp & (key_old ^ key_temp);
        key_old = key_temp;

        if (key_val != PLAT_KEY_STATE_NONE)
        {
            LOG_INFO("key down:%x", key_val);
        }

        vTaskDelayUntil(&start_tick, 30);
    }
}
