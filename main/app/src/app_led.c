#include "app_led.h"
#include "impl_led.h"
#include "ek_export.h"
#include "plat_led.h"

EK_LOG_FILE_TAG("app_led.c");

static void _app_led_task(void *arg);

TaskHandle_t g_led_handle;

void app_led_init(void)
{
    EK_LOG_INFO("ek_export: APP app_led_init");
    xTaskCreate(_app_led_task, "app led", 2048, NULL, 1, &g_led_handle);
}

EK_EXPORT_APP(app_led_init, 3);

static void _app_led_task(void *arg)
{
    while (1)
    {
        plat_led_dev_toggle(impl_led_r_dev());
        vTaskDelay(pdMS_TO_TICKS(500));

        plat_led_dev_toggle(impl_led_g_dev());
        vTaskDelay(pdMS_TO_TICKS(500));

        plat_led_dev_toggle(impl_led_b_dev());
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}
