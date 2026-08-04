#ifndef BSP_I2C_H
#define BSP_I2C_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "common_header.h"

#include "driver/gpio.h"
#include "driver/i2c_master.h"

// 新板：I2C0 (XL9555 + ES8388) 走 IO1(SDA)/IO2(SCL)，IO41/42 让给 I2S SCK/MCLK
#define BSP_I2C0_SDA_PIN GPIO_NUM_1
#define BSP_I2C0_SCL_PIN GPIO_NUM_2
#define BSP_I2C1_SDA_PIN GPIO_NUM_38
#define BSP_I2C1_SCL_PIN GPIO_NUM_39

void bsp_i2c_init(i2c_port_num_t port);
void bsp_i2c_dev_register(i2c_port_num_t port, uint32_t freq, uint16_t addr, i2c_master_dev_handle_t *dev);
esp_err_t bsp_i2c_write(i2c_master_dev_handle_t dev, const uint8_t *txbuffer, size_t size, int timemout);
esp_err_t
bsp_i2c_mem_write(i2c_master_dev_handle_t dev, uint8_t reg, const uint8_t *txbuffer, size_t size, int timeout);
esp_err_t bsp_i2c_read(i2c_master_dev_handle_t dev, uint8_t *rxbuffer, size_t size, int timeout);
esp_err_t bsp_i2c_mem_read(i2c_master_dev_handle_t dev, uint8_t reg, uint8_t *rxbuffer, size_t size, int timeout);

#ifdef __cplusplus
}
#endif

#endif //  BSP_I2C_H
