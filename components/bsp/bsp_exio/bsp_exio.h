#ifndef BSP_EXIO_H
#define BSP_EXIO_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "common_header.h"

// 超时等待时间
#define BSP_EXIO_TIMEOUT   100
#define BSP_EXIO_INT_STACK 2048

// GPIO 映射
#define BSP_EXIO_INT_IO   GPIO_NUM_0

#define BSP_EXIO_DEV_ADDR 0x20
// 输入寄存器
#define BSP_EXIO_INPUT_PORT0_REG 0
#define BSP_EXIO_INPUT_PORT1_REG 1
// 输出寄存器
#define BSP_EXIO_OUTPUT_PORT0_REG 2
#define BSP_EXIO_OUTPUT_PORT1_REG 3
// 极性翻转寄存器
#define BSP_EXIO_INVERSION_PORT0_REG 4
#define BSP_EXIO_INVERSION_PORT1_REG 5
// 方向配置寄存器
#define BSP_EXIO_DIRECTION_CFG_PORT0_REG 6
#define BSP_EXIO_DIRECTION_CFG_PORT1_REG 7

typedef enum
{
    BSP_EXIO_NUM0 = 0x0001UL,
    BSP_EXIO_NUM1 = 0x0002UL,
    BSP_EXIO_NUM2 = 0x0004UL,
    BSP_EXIO_NUM3 = 0x0008UL,
    BSP_EXIO_NUM4 = 0x0010UL,
    BSP_EXIO_NUM5 = 0x0020UL,
    BSP_EXIO_NUM6 = 0x0040UL,
    BSP_EXIO_NUM7 = 0x0080UL,
    BSP_EXIO_NUM8 = 0x0100UL,
    BSP_EXIO_NUM9 = 0x0200UL,
    BSP_EXIO_NUM10 = 0x0400UL,
    BSP_EXIO_NUM11 = 0x0800UL,
    BSP_EXIO_NUM12 = 0x1000UL,
    BSP_EXIO_NUM13 = 0x2000UL,
    BSP_EXIO_NUM14 = 0x4000UL,
    BSP_EXIO_NUM15 = 0x8000UL,

    BSP_EXIO_MAX,
} bsp_exio_pin_num_t;

typedef enum
{
    BSP_EXIO_PIN_MODE_INPUT = 0,
    BSP_EXIO_PIN_MODE_OUTPUT,

    BSP_EXIO_PIN_MODE_MAX,
} bsp_exio_pin_mode_t;

typedef struct
{
    bsp_exio_pin_num_t pin;
    bsp_exio_pin_mode_t mode;
} bsp_exio_pin_config_t;

// BSP_EXIO输出端口默认配置
// #define BSP_EXIO_DEFAULT_CONFIG 0xF003

// BSP_EXIO各个IO的功能
// #define BSP_AP_INT_IO   BSP_EXIO_NUM0
// #define BSP_QMA_INT_IO  BSP_EXIO_NUM1
// #define BSP_SPK_EN_IO   BSP_EXIO_NUM2
// #define BSP_BEEP_IO     BSP_EXIO_NUM3
// #define BSP_OV_PWON_IO  BSP_EXIO_NUM4
// #define BSP_OV_RESET_IO BSP_EXIO_NUM5
// #define BSP_GBC_LED_IO  BSP_EXIO_NUM6
// #define BSP_GBC_KEY_IO  BSP_EXIO_NUM7
// #define BSP_LCD_BL_IO   BSP_EXIO_NUM8
// #define BSP_CT_RST_IO   BSP_EXIO_NUM9
// #define BSP_SLCD_RST_IO BSP_EXIO_NUM10
// #define BSP_SLCD_PWR_IO BSP_EXIO_NUM11
// #define BSP_KEY3_IO     BSP_EXIO_NUM12
// #define BSP_KEY2_IO     BSP_EXIO_NUM13
// #define BSP_KEY1_IO     BSP_EXIO_NUM14
// #define BSP_KEY0_IO     BSP_EXIO_NUM15

void bsp_exio_init(void);
void bsp_exio_int_enable(void);
void bsp_exio_int_disable(void);
void bsp_exio_set_int_cb(void (*cb)(void));

esp_err_t bsp_exio_set_directoin(uint16_t value);
esp_err_t bsp_exio_get_directoin(uint16_t *value);

esp_err_t bsp_exio_read(uint16_t *value);
esp_err_t bsp_exio_write(uint16_t value);

void bsp_exio_conifg_pin(bsp_exio_pin_config_t *config);
void bsp_exio_write_pin(bsp_exio_pin_num_t pin, uint8_t value);

uint8_t bsp_exio_read_pin(bsp_exio_pin_num_t pin);

#ifdef __cplusplus
}
#endif

#endif // BSP_EXIO_H
