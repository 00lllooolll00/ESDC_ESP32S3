#include "bsp_i2c.h"

static const char *TAG = "bsp_i2c.c";

static i2c_master_bus_handle_t g_bsp_i2c0_handle = NULL;
static i2c_master_bus_handle_t g_bsp_i2c1_handle = NULL;

/**
 * @brief 初始化I2C主机总线
 *
 * 根据端口号配置并初始化I2C主机总线，支持I2C0和I2C1两个端口
 *
 * @param port I2C端口号 (I2C_NUM_0 或 I2C_NUM_1)
 */
void bsp_i2c_init(i2c_port_num_t port)
{
    i2c_master_bus_handle_t *i2c_init_handle = port == I2C_NUM_0 ? &g_bsp_i2c0_handle : &g_bsp_i2c1_handle;

    if (*i2c_init_handle != NULL) return;

    i2c_master_bus_config_t _i2c_config = {
        .i2c_port = port,
        .sda_io_num = port == I2C_NUM_0 ? BSP_I2C0_SDA_PIN : BSP_I2C1_SDA_PIN,
        .scl_io_num = port == I2C_NUM_0 ? BSP_I2C0_SCL_PIN : BSP_I2C1_SCL_PIN,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
    };

    ESP_ERROR_CHECK(i2c_new_master_bus(&_i2c_config, i2c_init_handle));
}

/**
 * @brief 在I2C总线上注册设备
 *
 * 向已初始化的I2C主机总线添加从设备
 *
 * @param port I2C端口号 (I2C_NUM_0 或 I2C_NUM_1)
 * @param freq I2C通信频率(Hz)
 * @param addr 从设备7位地址
 * @param[out] dev_hanlde 设备句柄指针，用于后续操作
 */
void bsp_i2c_dev_register(i2c_port_num_t port, uint32_t freq, uint16_t addr, i2c_master_dev_handle_t *dev)
{
    i2c_master_bus_handle_t *i2c_dev_reg_handle = port == I2C_NUM_0 ? &g_bsp_i2c0_handle : &g_bsp_i2c1_handle;

    i2c_device_config_t _dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = addr,
        .scl_speed_hz = freq,
    };
    ESP_ERROR_CHECK(i2c_master_bus_add_device(*i2c_dev_reg_handle, &_dev_cfg, dev));
}

/**
 * @brief I2C写数据
 *
 * 向I2C从设备发送数据
 *
 * @param dev_handle I2C设备句柄
 * @param txbuffer 待发送数据缓冲区指针
 * @param size 待发送数据长度
 * @param timemout 超时时间(ms)
 * @return esp_err_t ESP_OK成功，其他值失败
 */
esp_err_t bsp_i2c_write(i2c_master_dev_handle_t dev, const uint8_t *txbuffer, size_t size, int timemout)
{
    return i2c_master_transmit(dev, txbuffer, size, timemout);
}

/**
 * @brief I2C写寄存器
 *
 * 向I2C从设备的指定寄存器写入数据，格式为: [寄存器地址][数据...]
 *
 * @param dev I2C设备句柄
 * @param reg 寄存器地址
 * @param txbuffer 待写入数据缓冲区指针
 * @param size 待写入数据长度
 * @param timeout 超时时间(ms)
 * @return esp_err_t ESP_OK成功，其他值失败
 */
esp_err_t bsp_i2c_mem_write(i2c_master_dev_handle_t dev, uint8_t reg, const uint8_t *txbuffer, size_t size, int timeout)
{
    uint8_t *buffer = (uint8_t *)malloc(size + 1);
    if (buffer == NULL)
    {
        ESP_LOGE(TAG, "Failed to allocate memory");
        return ESP_ERR_NO_MEM;
    }
    buffer[0] = reg;
    memcpy(buffer + 1, txbuffer, size);
    esp_err_t err = i2c_master_transmit(dev, buffer, size + 1, timeout);
    free(buffer);
    return err;
}

/**
 * @brief I2C读数据
 *
 * 从I2C从设备接收数据
 *
 * @param dev I2C设备句柄
 * @param[out] rxbuffer 接收数据缓冲区指针
 * @param size 待接收数据长度
 * @param timeout 超时时间(ms)
 * @return esp_err_t ESP_OK成功，其他值失败
 */
esp_err_t bsp_i2c_read(i2c_master_dev_handle_t dev, uint8_t *rxbuffer, size_t size, int timeout)
{
    return i2c_master_receive(dev, rxbuffer, size, timeout);
}

/**
 * @brief I2C读寄存器
 *
 * 从I2C从设备的指定寄存器读取数据，先发送寄存器地址再接收数据
 *
 * @param dev I2C设备句柄
 * @param reg 寄存器地址
 * @param[out] rxbuffer 接收数据缓冲区指针
 * @param size 待接收数据长度
 * @param timeout 超时时间(ms)
 * @return esp_err_t ESP_OK成功，其他值失败
 */
esp_err_t bsp_i2c_mem_read(i2c_master_dev_handle_t dev, uint8_t reg, uint8_t *rxbuffer, size_t size, int timeout)
{
    return i2c_master_transmit_receive(dev, &reg, 1, rxbuffer, size, timeout);
}
