#include "bsp_exio.h"
#include "bsp_i2c.h"

FILE_TAG("bsp_exio.c");

static esp_err_t _exio_i2c_write(uint8_t reg, const uint8_t *txbuffer, size_t size);
static esp_err_t _exio_i2c_read(uint8_t reg, uint8_t *rxbuffer, size_t size);
static void _exio_exti_cb(void *arg);

static i2c_master_dev_handle_t g_exio_i2c_handle = NULL;
static TaskHandle_t g_exio_int_task_handle = NULL;

void (*bsp_exio_int_cb)(void);

static void bsp_exio_int_task(void *arg)
{
    while (1)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        uint16_t value;
        bsp_exio_read(&value); // 读取清除中断标志位
        if (bsp_exio_int_cb)
        {
            bsp_exio_int_cb();
        }
    }
}

/**
 * @brief 初始化XL9555 IO扩展芯片
 *
 * 初始化I2C总线并注册XL9555设备，设置默认的GPIO方向配置
 */
void bsp_exio_init(void)
{
    bsp_i2c_init(I2C_NUM_0);
    bsp_i2c_dev_register(I2C_NUM_0, EK_FREQ_K(400), BSP_EXIO_DEV_ADDR, &g_exio_i2c_handle);

    // 上电后读取一次清除中断
    uint16_t temp;

    ESP_ERROR_CHECK(bsp_exio_read(&temp));

    // bsp_exio_set_directoin(BSP_EXIO_DEFAULT_CONFIG);

    LOG_INFO("bsp exio init ok");
}

/**
 * @brief 使能XL9555中断功能
 *
 * 配置XL9555中断引脚为输入模式，启用上拉电阻，设置下降沿触发中断，
 * 并注册中断回调函数
 */
void bsp_exio_int_enable(void)
{
    gpio_config_t bsp_exio_int_config = {
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_NEGEDGE,
        .pin_bit_mask = (1ULL << BSP_EXIO_INT_IO),
    };

    ESP_ERROR_CHECK(gpio_config(&bsp_exio_int_config));
    esp_err_t ret = gpio_install_isr_service(0);
    if (ret != ESP_OK && ret != ESP_ERR_INVALID_STATE)
    {
        ESP_ERROR_CHECK(ret);
    }
    xTaskCreate(bsp_exio_int_task, "bsp_exio_int", BSP_EXIO_INT_STACK, NULL, 10, &g_exio_int_task_handle);
    ESP_ERROR_CHECK(gpio_isr_handler_add(BSP_EXIO_INT_IO, _exio_exti_cb, (void *)BSP_EXIO_INT_IO));
}

/**
 * @brief 禁用XL9555中断功能
 *
 * 移除中断回调函数并禁用中断引脚
 */
void bsp_exio_int_disable(void)
{
    ESP_ERROR_CHECK(gpio_isr_handler_remove(BSP_EXIO_INT_IO));
    gpio_config_t bsp_exio_int_config = {
        .mode = GPIO_MODE_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
        .pin_bit_mask = (1ULL << BSP_EXIO_INT_IO),
    };
    ESP_ERROR_CHECK(gpio_config(&bsp_exio_int_config));
}

/**
 * @brief 设置 XL9555 的中断回调函数
 *
 * @param cb 中断回调函数
 */
void bsp_exio_set_int_cb(void (*cb)(void))
{
    bsp_exio_int_cb = cb;
}

/**
 * @brief 设置XL9555 GPIO方向
 *
 * @param value 方向配置值，对应位为1表示输入，为0表示输出
 * @return esp_err_t ESP_OK成功，其他值失败
 */
esp_err_t bsp_exio_set_directoin(uint16_t value)
{
    uint8_t buf[2] = { 0 };
    buf[0] = value & 0x00ff;
    buf[1] = (value & 0xff00) >> 8;

    return _exio_i2c_write(BSP_EXIO_DIRECTION_CFG_PORT0_REG, (const uint8_t *)buf, 2);
}

/**
 * @brief 获取XL9555 GPIO方向配置
 *
 * @param[out] value 用于存储方向配置值的指针
 * @return esp_err_t ESP_OK成功，其他值失败
 */
esp_err_t bsp_exio_get_directoin(uint16_t *value)
{
    return _exio_i2c_read(BSP_EXIO_DIRECTION_CFG_PORT0_REG, (uint8_t *)value, 2);
}

/**
 * @brief 读取XL9555所有GPIO输入状态
 *
 * @param[out] value 用于存储输入状态值的指针
 * @return esp_err_t ESP_OK成功，其他值失败
 */
esp_err_t bsp_exio_read(uint16_t *value)
{
    return _exio_i2c_read(BSP_EXIO_INPUT_PORT0_REG, (uint8_t *)value, 2);
}

/**
 * @brief 写入XL9555所有GPIO输出状态
 *
 * @param value 输出状态值，对应位为1输出高电平，为0输出低电平
 * @return esp_err_t ESP_OK成功，其他值失败
 */
esp_err_t bsp_exio_write(uint16_t value)
{
    uint8_t buf[2] = { 0 };
    buf[0] = (uint8_t)(value & 0x00ff);
    buf[1] = (uint8_t)((value & 0xff00) >> 8);

    return _exio_i2c_write(BSP_EXIO_OUTPUT_PORT0_REG, buf, 2);
}

/**
 * @brief 配置XL9555单个GPIO引脚模式
 *
 * @param config 引脚配置结构体指针，包含引脚号和模式
 */
void bsp_exio_conifg_pin(bsp_exio_pin_config_t *config)
{
    uint16_t directon_value = 0;
    ESP_ERROR_CHECK(bsp_exio_get_directoin(&directon_value));

    uint16_t cfg_value = directon_value;
    if (config->mode == BSP_EXIO_PIN_MODE_OUTPUT) cfg_value &= ~(config->pin);
    else cfg_value |= config->pin;

    ESP_ERROR_CHECK(bsp_exio_set_directoin(cfg_value));
}

/**
 * @brief 设置XL9555单个GPIO引脚输出电平
 *
 * @param pin 引脚号枚举值
 * @param value 输出电平，0为低电平，非0为高电平
 */
void bsp_exio_write_pin(bsp_exio_pin_num_t pin, uint8_t value)
{
    uint16_t old_value = 0;
    ESP_ERROR_CHECK(bsp_exio_read(&old_value));
    uint16_t new_value = old_value;
    if (value) new_value |= pin;
    else new_value &= ~pin;
    ESP_ERROR_CHECK(bsp_exio_write(new_value));
}

/**
 * @brief 读取XL9555单个GPIO引脚输入状态
 *
 * @param pin 引脚号枚举值
 * @return uint8_t 引脚状态，0为低电平，1为高电平
 */
uint8_t bsp_exio_read_pin(bsp_exio_pin_num_t pin)
{
    uint16_t value = 0;
    ESP_ERROR_CHECK(bsp_exio_read(&value));
    return (value & pin) != 0;
}

/**
 * @brief XL9555 I2C写寄存器
 *
 * @param reg 寄存器地址
 * @param txbuffer 待写入数据缓冲区指针
 * @param size 待写入数据长度
 * @return esp_err_t ESP_OK成功，其他值失败
 */
static esp_err_t _exio_i2c_write(uint8_t reg, const uint8_t *txbuffer, size_t size)
{
    if (g_exio_i2c_handle == NULL) return ESP_ERR_NOT_FOUND;
    return bsp_i2c_mem_write(g_exio_i2c_handle, reg, txbuffer, size, BSP_EXIO_TIMEOUT);
}

/**
 * @brief XL9555 I2C读寄存器
 *
 * @param reg 寄存器地址
 * @param[out] rxbuffer 接收数据缓冲区指针
 * @param size 待读取数据长度
 * @return esp_err_t ESP_OK成功，其他值失败
 */
static esp_err_t _exio_i2c_read(uint8_t reg, uint8_t *rxbuffer, size_t size)
{
    if (g_exio_i2c_handle == NULL) return ESP_ERR_NOT_FOUND;
    return bsp_i2c_mem_read(g_exio_i2c_handle, reg, rxbuffer, size, BSP_EXIO_TIMEOUT);
}

/**
 * @brief XL9555外部中断回调函数
 *
 * 当检测到中断引脚低电平时调用用户定义的中断回调函数
 *
 * @param arg 未使用的参数
 */
static void _exio_exti_cb(void *arg)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    vTaskNotifyGiveFromISR(g_exio_int_task_handle, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}
