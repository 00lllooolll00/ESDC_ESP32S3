#include "common_header.h"
#include "app.h"
#include "nvs_flash.h"
#include "lvgl.h"

void app_main(void)
{
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ESP_ERROR_CHECK(nvs_flash_init());
    }

    app_init();
}

void vApplicationTickHook(void)
{
    lv_tick_inc(1);
}
