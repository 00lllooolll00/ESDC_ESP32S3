#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "bsp_led.h"

void app_main(void)
{
    bsp_led_init();
    while (1)
    {
        BSP_LED_TOGGLE();
        vTaskDelay(500);
    }
}
