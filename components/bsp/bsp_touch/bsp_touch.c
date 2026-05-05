#include "bsp_touch.h"
#include "bsp_i2c.h"

FILE_TAG("bsp_touch.c");

FORCE_INLINE_ATTR esp_err_t _touch_write_reg(uint16_t reg, uint8_t *txdata, uint8_t size);
FORCE_INLINE_ATTR esp_err_t _touch_read_reg(uint16_t reg, uint8_t *rxdata, uint8_t size);
static void _touch_exio_rst_pin_config(void);
static void IRAM_ATTR _touch_exti_cb(void *arg);

static i2c_master_dev_handle_t s_touch_dev_handle;
static TaskHandle_t s_touch_int_task;
static void (*s_touch_int_cb)(void);

static void bsp_touch_int_task(void *arg)
{
    while (1)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        uint16_t value;
        bsp_exio_read(&value); // 读取清除中断标志位
        if (s_touch_int_cb)
        {
            s_touch_int_cb();
        }
    }
}

void bsp_touch_init(void)
{
    bsp_i2c_init(I2C_NUM_1);
    bsp_i2c_dev_register(I2C_NUM_1, EK_FREQ_K(400), BSP_TOUCH_DEV_ADDR, &s_touch_dev_handle);
    _touch_exio_rst_pin_config();
    bsp_touch_int_disable();

    for (int i = 2; i > 0; i--)
    {
        BSP_TOUCH_CT_RST(0);
        vTaskDelay(pdMS_TO_TICKS(200));
        BSP_TOUCH_CT_RST(1);
        vTaskDelay(pdMS_TO_TICKS(200));
    }

    vTaskDelay(pdMS_TO_TICKS(100));

    uint8_t temp[2] = { 0 };

    temp[0] = 0x02;
    ESP_ERROR_CHECK(_touch_write_reg(BSP_TOUCH_CTRL_REG, temp, 1));

    vTaskDelay(pdMS_TO_TICKS(10));

    temp[0] = 0x00;
    ESP_ERROR_CHECK(_touch_write_reg(BSP_TOUCH_CTRL_REG, temp, 1));
}

void bsp_touch_int_enable(void (*cb)(void))
{
    if (s_touch_int_cb)
    {
        LOG_WARN("there is already a callback of touch");
        return;
    }

    gpio_config_t gpio_init_struct = { 0 };
    gpio_init_struct.intr_type = GPIO_INTR_ANYEDGE;
    gpio_init_struct.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_init_struct.pull_down_en = GPIO_PULLDOWN_DISABLE;
    gpio_init_struct.mode = GPIO_MODE_INPUT;
    gpio_init_struct.pin_bit_mask = 1ull << BSP_TOUCH_INT_PIN;
    ESP_ERROR_CHECK(gpio_config(&gpio_init_struct));
    ESP_ERROR_CHECK(gpio_install_isr_service(ESP_INTR_FLAG_IRAM));
    ESP_ERROR_CHECK(gpio_isr_handler_add(BSP_TOUCH_INT_PIN, _touch_exti_cb, NULL));

    if (!s_touch_int_task)
    {
        xTaskCreate(bsp_touch_int_task, "touch int", BSP_TOUCH_INT_STACK, NULL, 10, &s_touch_int_task);
    }
    s_touch_int_cb = cb;
}

void bsp_touch_int_disable(void)
{
    gpio_config_t gpio_init_struct = { 0 };
    gpio_init_struct.intr_type = GPIO_INTR_DISABLE;
    gpio_init_struct.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_init_struct.pull_down_en = GPIO_PULLDOWN_DISABLE;
    gpio_init_struct.mode = GPIO_MODE_INPUT;
    gpio_init_struct.pin_bit_mask = 1ull << BSP_TOUCH_INT_PIN;
    ESP_ERROR_CHECK(gpio_config(&gpio_init_struct));
}

void bsp_touch_scan(bsp_touch_points_t *points)
{
    uint8_t buf[8 * BSP_TOUCH_POINT_NUM] = { 0 };
    points->count = 0;

    /* 读取 GSTID 寄存器: bit7=buffer状态, bit3:0=触摸点数 */
    _touch_read_reg(BSP_TOUCH_GSTID_REG, buf, 1);
    uint8_t touch_num = buf[0] & 0x0F;

    if (touch_num == 0 || touch_num > BSP_TOUCH_POINT_NUM)
    {
        goto clear;
    }

    /* 批量读取所有触摸点数据，每个点8字节 */
    _touch_read_reg(BSP_TOUCH_TP1_REG, buf, touch_num * 8);

    /* 解析触摸点: track_id(1) + x_h(1) + x_l(1) + y_h(1) + y_l(1) + size(2) + reserved(1) */
    for (int i = 0; i < touch_num; i++)
    {
        points->points[i].x = ((uint16_t)buf[i * 8 + 1] << 8) | buf[i * 8 + 2];
        points->points[i].y = ((uint16_t)buf[i * 8 + 3] << 8) | buf[i * 8 + 4];
    }
    points->count = touch_num;

clear:
    /* 清除 GSTID 寄存器 */
    buf[0] = 0;
    _touch_write_reg(BSP_TOUCH_GSTID_REG, buf, 1);
}

FORCE_INLINE_ATTR esp_err_t _touch_write_reg(uint16_t reg, uint8_t *txdata, uint8_t size)
{
    uint8_t *tx = malloc(size + 2);
    assert(tx && "buy more ram lol");
    tx[0] = reg >> 8;
    tx[1] = reg & 0xFF;
    memcpy(tx + 2, txdata, size);

    esp_err_t err = bsp_i2c_write(s_touch_dev_handle, tx, size + 2, BSP_TOUCH_TIMEOUT);

    free(tx);

    return err;
}

FORCE_INLINE_ATTR esp_err_t _touch_read_reg(uint16_t reg, uint8_t *rxdata, uint8_t size)
{
    uint8_t rx[2] = { 0 };
    rx[0] = reg >> 8;
    rx[1] = reg & 0xFF;
    return i2c_master_transmit_receive(s_touch_dev_handle, rx, 2, rxdata, size, BSP_TOUCH_TIMEOUT);
}

static void _touch_exio_rst_pin_config(void)
{
    bsp_exio_pin_config_t exio_lcd_conifg = {
        .mode = BSP_EXIO_PIN_MODE_OUTPUT,
        .pin = BSP_TOUCH_RST_PIN,
    };
    bsp_exio_conifg_pin(&exio_lcd_conifg);
    exio_lcd_conifg.pin = BSP_TOUCH_RST_PIN;
    bsp_exio_conifg_pin(&exio_lcd_conifg);
}

static void IRAM_ATTR _touch_exti_cb(void *arg)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    vTaskNotifyGiveFromISR(s_touch_int_task, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}
