#ifndef BSP_SPI_H
#define BSP_SPI_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "common_header.h"
#include "driver/gpio.h"
#include "driver/spi_master.h"

#define BSP_SPI_HOST          SPI2_HOST
// 新板 SPI 总线未分配引脚（IO11/12/13 已分配给 LCD），如需使用请根据实际硬件重新分配
#define BSP_SPI_SCLK_PIN      GPIO_NUM_NC
#define BSP_SPI_MOSI_PIN      GPIO_NUM_NC
#define BSP_SPI_MISO_PIN      GPIO_NUM_NC

#define BSP_SPI_MAX_BUFF_SIZE (320 * 240 * sizeof(uint16_t))

void bsp_spi_init(void);

#ifdef __cplusplus
}
#endif

#endif //  BSP_SPI_H
