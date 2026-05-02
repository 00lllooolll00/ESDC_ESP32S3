#include "bsp_sdcard.h"
#include "bsp_spi.h"
#include "driver/sdspi_host.h"
#include "driver/sdmmc_host.h"

FILE_TAG("bsp_sdcard.c");

esp_err_t bsp_sdcard_init(void)
{
    bsp_spi_init();

    sdmmc_card_t *sdcard = NULL;
    esp_vfs_fat_sdcard_unmount(BSP_SDCARD_PATH, sdcard);

    esp_vfs_fat_sdmmc_mount_config_t mount_cfg = {
        .format_if_mount_failed = false,
        .max_files = 5,
        .allocation_unit_size = 4 * 1024 * sizeof(uint8_t),
    };

    sdmmc_host_t host = SDSPI_HOST_DEFAULT();

    sdspi_device_config_t slot_cfg = {
        .host_id = host.slot,
        .gpio_cs = BSP_SDCARD_CS_PIN,
        .gpio_cd = GPIO_NUM_NC,
        .gpio_int = GPIO_NUM_NC,
        .gpio_wp = GPIO_NUM_NC,
    };
    esp_err_t err = 0;
    uint8_t retry = 0;

    while (retry <= BSP_SDCARD_MAX_RETRY)
    {
        if (retry > 0)
        {
            LOG_WARN("retry to mount the sd card:%d/%d", retry, BSP_SDCARD_MAX_RETRY);
            vTaskDelay(pdMS_TO_TICKS(100));
        }

        err = esp_vfs_fat_sdspi_mount(BSP_SDCARD_PATH, &host, &slot_cfg, &mount_cfg, &sdcard);

        if (err == ESP_OK)
        {
            LOG_INFO("SD card mounted successfully!");
            break;
        }

        LOG_WARN("fail to mount: %s", esp_err_to_name(err));

        if (err == ESP_FAIL)
        {
            LOG_WARN("try to format the sdcard...");
            err = esp_vfs_fat_sdcard_format(BSP_SDCARD_PATH, sdcard);
            if (err == ESP_OK)
            {
                LOG_INFO("format and mount successfully!");
                break;
            }
            else
            {
                LOG_WARN("fail to format the sd card: %s", esp_err_to_name(err));
            }
        }
        else if (err == ESP_ERR_NOT_FOUND)
        {
            LOG_WARN("there is not a sd card found");
            break;
        }

        retry++;
    }

    return err;
}

void bsp_sdcard_get_usage(size_t *out_total_bytes, size_t *out_free_bytes)
{
    FATFS *fs;
    size_t free_clusters;
    int res = f_getfree("0:", (DWORD *)&free_clusters, &fs);
    assert(res == FR_OK);
    size_t total_sectors = (fs->n_fatent - 2) * fs->csize;
    size_t free_sectors = free_clusters * fs->csize;

    size_t sd_total = total_sectors / 1024;
    size_t sd_total_KB = sd_total * fs->ssize;
    size_t sd_free = free_sectors / 1024;
    size_t sd_free_KB = sd_free * fs->ssize;

    if (out_total_bytes != NULL)
    {
        *out_total_bytes = sd_total_KB;
    }

    if (out_free_bytes != NULL)
    {
        *out_free_bytes = sd_free_KB;
    }
}
