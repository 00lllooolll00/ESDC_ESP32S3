#include "common_header.h"

EK_LOG_FILE_TAG("bsp_spi.c");

#include "bsp_spi.h"

volatile static bool s_init_flag;
/**
 * @brief       SPI总线初始化
 * @param       无
 * @retval      无
 */
void bsp_spi_init(void)
{
    if (s_init_flag) return;

    // 新板 SPI 总线未分配引脚，跳过初始化避免 spi_bus_initialize 传入 NC 崩溃
    if (BSP_SPI_SCLK_PIN == GPIO_NUM_NC)
    {
        EK_LOG_WARN("bsp_spi_init skipped: SPI pins not assigned on this board revision");
        s_init_flag = true;
        return;
    }

    s_init_flag = true;

    spi_bus_config_t buscfg = {
        .sclk_io_num = BSP_SPI_SCLK_PIN,
        .mosi_io_num = BSP_SPI_MOSI_PIN,
        .miso_io_num = BSP_SPI_MISO_PIN,
        .quadhd_io_num = -1,
        .quadwp_io_num = -1,
        .max_transfer_sz = BSP_SPI_MAX_BUFF_SIZE,
    };

    ESP_ERROR_CHECK(spi_bus_initialize(BSP_SPI_HOST, &buscfg, SPI_DMA_CH_AUTO));
}
