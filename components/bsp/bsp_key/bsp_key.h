#ifndef BSP_KEY_H
#define BSP_KEY_H

#include "common_header.h"
#include "driver/gpio.h"
#include "bsp_exio.h"

#define BSP_KEY_BOOT_PIN GPIO_NUM_0
#define BSP_KEY0_PIN     BSP_EXIO_NUM15
#define BSP_KEY1_PIN     BSP_EXIO_NUM14
#define BSP_KEY2_PIN     BSP_EXIO_NUM13
#define BSP_KEY3_PIN     BSP_EXIO_NUM12

typedef enum
{
    BSP_KEY_STATE_NONE = 0,
    BSP_KEY_STATE_BOOT = 0x01,
    BSP_KEY_STATE_0 = 0x02,
    BSP_KEY_STATE_1 = 0x04,
    BSP_KEY_STATE_2 = 0x08,
    BSP_KEY_STATE_3 = 0x10,

    BSP_KEY_STATE_MAX = 0x1F,
} bsp_key_state_t;

void bsp_key_init(void);
bsp_key_state_t bsp_key_read_raw(void);

#endif // BSP_KEY_H
