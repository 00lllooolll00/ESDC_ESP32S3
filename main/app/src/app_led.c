#include "app_led.h"
#include "plat_led.h"

FILE_TAG("app_led.c");

extern plat_led_dev_t g_led_dev;

static void _app_led_task(void *arg);

TaskHandle_t g_led_handle;

void app_led_init(void)
{
    xTaskCreate(_app_led_task, "app led", 1024, NULL, 1, &g_led_handle);
}

static void _app_led_task(void *arg)
{
    while (1)
    {
        plat_led_dev_toggle(&g_led_dev);
        vTaskDelay(500);
    }
}
