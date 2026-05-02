#ifndef BSP_LCD_H
#define BSP_LCD_H

#include "common_header.h"
#include "bsp_exio.h"

#define BSP_LCD_SPI_HOST    SPI2_HOST

#define BSP_LCD_WIDTH       320
#define BSP_LCD_HEIGHT      240

#define BSP_LCD_BUFFER_SIZE (BSP_LCD_WIDTH * BSP_LCD_HEIGHT * sizeof(uint16_t))

#define BSP_LCD_SCLK_PIN    GPIO_NUM_12
#define BSP_LCD_MOSI_PIN    GPIO_NUM_11
#define BSP_LCD_MISO_PIN    GPIO_NUM_13
#define BSP_LCD_RST_PIN     BSP_EXIO_NUM10
#define BSP_LCD_PWR_PIN     BSP_EXIO_NUM11
#define BSP_LCD_DC_PIN      GPIO_NUM_40
#define BSP_LCD_CS_PIN      GPIO_NUM_21

typedef bool (*bsp_lcd_trans_done_cb_t)(void *);

#define BSP_LCD_PWR(x) bsp_exio_write_pin(BSP_LCD_PWR_PIN, x)
#define BSP_LCD_RST(x) bsp_exio_write_pin(BSP_LCD_RST_PIN, x)

void bsp_lcd_init(bsp_lcd_trans_done_cb_t cb, void *arg);
void bsp_lcd_clear(uint16_t color);
void bsp_lcd_disp_flush(int16_t x0, int16_t y0, int16_t x1, int16_t y1, const void *buffer);

#endif // BSP_LCD_H
