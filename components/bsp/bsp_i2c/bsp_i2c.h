#ifndef BSP_I2C_H
#define BSP_I2C_H

#include "common_header.h"

#include "driver/gpio.h"
#include "driver/i2c_master.h"

#define BSP_I2C0_SDA_PIN GPIO_NUM_41
#define BSP_I2C0_SCL_PIN GPIO_NUM_42
#define BSP_I2C1_SDA_PIN GPIO_NUM_5
#define BSP_I2C1_SCL_PIN GPIO_NUM_4

void bsp_i2c_init(i2c_port_num_t port);
void bsp_i2c_dev_register(i2c_port_num_t port, uint32_t freq, uint16_t addr, i2c_master_dev_handle_t *dev);
esp_err_t bsp_i2c_write(i2c_master_dev_handle_t dev, const uint8_t *txbuffer, size_t size, int timemout);
esp_err_t
bsp_i2c_mem_write(i2c_master_dev_handle_t dev, uint8_t reg, const uint8_t *txbuffer, size_t size, int timeout);
esp_err_t bsp_i2c_read(i2c_master_dev_handle_t dev, uint8_t *rxbuffer, size_t size, int timeout);
esp_err_t bsp_i2c_mem_read(i2c_master_dev_handle_t dev, uint8_t reg, uint8_t *rxbuffer, size_t size, int timeout);

#endif //  BSP_I2C_H
