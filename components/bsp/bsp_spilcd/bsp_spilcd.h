#ifndef BSP_SPILCD_H
#define BSP_SPILCD_H

#include "common_header.h"
#include "bsp_exio.h"

#define BSP_SPILCD_WIDTH       320
#define BSP_SPILCD_HEIGHT      240

#define BSP_SPILCD_BUFFER_SIZE (BSP_LCD_WIDTH * BSP_LCD_HEIGHT * sizeof(uint16_t))

#define BSP_SPILCD_RST_PIN     BSP_EXIO_NUM10
#define BSP_SPILCD_PWR_PIN     BSP_EXIO_NUM11
#define BSP_SPILCD_DC_PIN      GPIO_NUM_40
#define BSP_SPILCD_CS_PIN      GPIO_NUM_21

typedef bool (*bsp_lcd_trans_done_cb_t)(void *);

#define BSP_SPILCD_PWR(x) bsp_exio_write_pin(BSP_SPILCD_PWR_PIN, x)
#define BSP_SPILCD_RST(x) bsp_exio_write_pin(BSP_SPILCD_RST_PIN, x)

void bsp_spilcd_init(bsp_lcd_trans_done_cb_t cb, void *arg);
void bsp_spilcd_clear(uint16_t color);
void bsp_spilcd_disp_flush(int16_t x0, int16_t y0, int16_t x1, int16_t y1, const void *buffer);

#endif // BSP_SPILCD_H
