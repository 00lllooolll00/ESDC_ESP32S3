#include "app_key.h"
#include "impl_key.h"
#include "ek_export.h"
#include "plat_key.h"

EK_LOG_FILE_TAG("app_key.c");

static void _app_key_task(void *arg);

TaskHandle_t g_key_handle;

void app_key_intit(void)
{
    EK_LOG_INFO("ek_export: APP app_key_intit");
    xTaskCreate(_app_key_task, "app key", 2048, NULL, 2, &g_key_handle);
}

EK_EXPORT_APP(app_key_intit, 3);

static void _app_key_task(void *arg)
{
    plat_key_state_t key_old = PLAT_KEY_STATE_NONE;
    TickType_t start_tick = xTaskGetTickCount();
    while (1)
    {
        plat_key_state_t key_temp;
        plat_key_dev_read_raw(impl_key_dev(), &key_temp);
        plat_key_state_t key_val = key_temp & (key_old ^ key_temp);
        key_old = key_temp;

        if (key_val != PLAT_KEY_STATE_NONE)
        {
            EK_LOG_INFO("key down:%x", key_val);
        }

        vTaskDelayUntil(&start_tick, 30);
    }
}
