#include "common_header.h"
#include "app.h"
#include "nvs_flash.h"
#include "lvgl.h"
#include "esp_timer.h"

FILE_TAG("main.c");

static uint32_t _lv_port_tick_get_cb(void);

void app_main(void)
{
    portDISABLE_INTERRUPTS();
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        LOG_ERROR("fail to init nvs flash:%s", esp_err_to_name(err));
        ESP_ERROR_CHECK(nvs_flash_erase());
        ESP_ERROR_CHECK(nvs_flash_init());
        LOG_INFO("successfully reinit nvs flash");
    }
    portENABLE_INTERRUPTS();
    lv_init();
    lv_tick_set_cb(_lv_port_tick_get_cb);
    app_init();
}

static uint32_t _lv_port_tick_get_cb(void)
{
    return (uint32_t)(esp_timer_get_time() / 1000);
}
