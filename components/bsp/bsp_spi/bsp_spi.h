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
#define BSP_SPI_SCLK_PIN      GPIO_NUM_12
#define BSP_SPI_MOSI_PIN      GPIO_NUM_11
#define BSP_SPI_MISO_PIN      GPIO_NUM_13

#define BSP_SPI_MAX_BUFF_SIZE (320 * 240 * sizeof(uint16_t))

void bsp_spi_init(void);

#ifdef __cplusplus
}
#endif

#endif //  BSP_SPI_H
