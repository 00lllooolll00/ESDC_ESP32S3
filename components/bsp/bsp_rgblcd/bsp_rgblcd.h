#ifndef BSP_LCD_H
#define BSP_LCD_H

#include "common_header.h"
#include "bsp_exio.h"
#include "driver/gpio.h"

#define BSP_RGBLCD_WIDTH  800
#define BSP_RGBLCD_HEIGHT 480

// pin
#define BSP_RGBLCD_DE_PIN    (GPIO_NUM_4)
#define BSP_RGBLCD_VSYNC_PIN (GPIO_NUM_NC)
#define BSP_RGBLCD_HSYNC_PIN (GPIO_NUM_NC)
#define BSP_RGBLCD_PCLK_PIN  (GPIO_NUM_5)

#define BSP_RGBLCD_R3_PIN    (GPIO_NUM_45)
#define BSP_RGBLCD_R4_PIN    (GPIO_NUM_48)
#define BSP_RGBLCD_R5_PIN    (GPIO_NUM_47)
#define BSP_RGBLCD_R6_PIN    (GPIO_NUM_21)
#define BSP_RGBLCD_R7_PIN    (GPIO_NUM_14)

#define BSP_RGBLCD_G2_PIN    (GPIO_NUM_10)
#define BSP_RGBLCD_G3_PIN    (GPIO_NUM_9)
#define BSP_RGBLCD_G4_PIN    (GPIO_NUM_46)
#define BSP_RGBLCD_G5_PIN    (GPIO_NUM_3)
#define BSP_RGBLCD_G6_PIN    (GPIO_NUM_8)
#define BSP_RGBLCD_G7_PIN    (GPIO_NUM_18)

#define BSP_RGBLCD_B3_PIN    (GPIO_NUM_17)
#define BSP_RGBLCD_B4_PIN    (GPIO_NUM_16)
#define BSP_RGBLCD_B5_PIN    (GPIO_NUM_15)
#define BSP_RGBLCD_B6_PIN    (GPIO_NUM_7)
#define BSP_RGBLCD_B7_PIN    (GPIO_NUM_6)

#define BSP_RGBLCD_BL_PIN    BSP_EXIO_NUM8
#define BSP_RGBLCD_BL(x)                                                                         \
    do                                                                                           \
    {                                                                                            \
        x ? bsp_exio_write_pin(BSP_RGBLCD_BL_PIN, 1) : bsp_exio_write_pin(BSP_RGBLCD_BL_PIN, 0); \
    } while (0)

typedef bool (*bsp_rgblcd_trans_done_cb_t)(void *);

void bsp_rgblcd_init(bsp_rgblcd_trans_done_cb_t cb, void *arg);
void bsp_rgblcd_clear(uint16_t color);
void bsp_rgblcd_disp_flush(int16_t x0, int16_t y0, int16_t x1, int16_t y1, const void *buffer);
uint16_t bsp_rgblcd_get_width(void);
uint16_t bsp_rgblcd_get_height(void);

#endif // BSP_LCD_H
