#include "app_key.h"
#include "plat_key.h"

FILE_TAG("app_key.c");

extern plat_key_dev_t g_key_dev;

static void _app_key_task(void *arg);

TaskHandle_t g_key_handle;

void app_key_intit(void)
{
    xTaskCreate(_app_key_task, "app key", 2048, NULL, 2, &g_key_handle);
}

static void _app_key_task(void *arg)
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
