// esp-idf
#include "nvs_flash.h"
#include "esp_vfs_fat.h"
#include "esp_pm.h"

// common（EK_LOG_FILE_TAG / LOG_* / unified_strerror）
#include "common_header.h"

// ek_utils 自动初始化
#include "ek_export.h"

EK_LOG_FILE_TAG("main.c");

// 启动总线：仅保留 NVS / VFS / PM 这三项必须在自动初始化前完成的基础设施，
// 其余所有外设注册、设备 init、LVGL 端口、应用任务均由各模块自行 EK_EXPORT_*
// 注册，ek_export_init() 按 level→order 自动调度。main.c 不感知任何具体设备。
void app_main(void)
{
    portDISABLE_INTERRUPTS();
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        EK_LOG_ERROR("fail to init nvs flash:%s", unified_strerror(err));
        ESP_ERROR_CHECK(nvs_flash_erase());
        ESP_ERROR_CHECK(nvs_flash_init());
        EK_LOG_INFO("successfully reinit nvs flash");
    }
    portENABLE_INTERRUPTS();

    const esp_vfs_fat_mount_config_t vfs_mount_cfg = {
        .format_if_mount_failed = false,
        .max_files = 4,
        .allocation_unit_size = 4096,
    };
    esp_err_t vfs_err = esp_vfs_fat_spiflash_mount_ro("/vfs", "vfs", &vfs_mount_cfg);
    if (vfs_err != ESP_OK)
    {
        EK_LOG_ERROR("fail to mount vfs: %s", unified_strerror(vfs_err));
    }
    else
    {
        EK_LOG_INFO("vfs mounted at /vfs");
    }

    // 配置电源管理：启用 DFS 动态调频与自动 light sleep
    // 必须在持有 PM lock 的外设（RGB LCD 等）初始化之前调用，否则 PM 模块不会真正生效
    esp_pm_config_esp32s3_t pm_config = {
        .max_freq_mhz = 240,
        .min_freq_mhz = 80,
        .light_sleep_enable = true,
    };
    ESP_ERROR_CHECK(esp_pm_configure(&pm_config));

    // 执行所有 EK_EXPORT_* 注册的初始化函数（按 level→order 排序后逐个调用）
    // HARDWARE:      impl_exio_init
    // COMPONENTS:    led/key/rgblcd/touch/wifi/tts register → led/key dev_init
    // APP:           lv_init+tick → disp/touch port → weather → wifi/led/key → ui → mqtt → console
    ek_export_init();
}
