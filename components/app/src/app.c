#include "app.h"
#include "bsp_led.h"

static void app_led_task(void *arg);

TaskHandle_t led_handle;

void app_init(void)
{
    bsp_led_init();

    xTaskCreate(app_led_task, "app led", 256, NULL, 1, &led_handle);
}

static void app_led_task(void *arg)
{
    while (1)
    {
        bsp_led_toggle();
        vTaskDelay(500);
    }
}
