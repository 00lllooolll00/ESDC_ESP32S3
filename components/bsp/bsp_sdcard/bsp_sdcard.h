#ifndef BSP_SDCARD_H
#define BSP_SDCARD_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "common_header.h"
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"
#include "driver/gpio.h"

#define BSP_SDCARD_PATH      "/0:"
#define BSP_SDCARD_MAX_RETRY 3
// 新板 SD 卡不可用：SPI 总线引脚未分配（见 bsp_spi.h），CS 脚标记为 NC
#define BSP_SDCARD_CS_PIN    GPIO_NUM_NC

esp_err_t bsp_sdcard_init(void);
void bsp_sdcard_get_usage(size_t *out_total_bytes, size_t *out_free_bytes);

#ifdef __cplusplus
}
#endif

#endif // BSP_SDCARD_H
