#ifndef BSP_I2S_H
#define BSP_I2S_H

#include "common_header.h"
#include "driver/gpio.h"
#include "driver/i2s_std.h"

#define BSP_I2S               (I2S_NUM_0)
#define BSP_I2S_BCK_IO        (GPIO_NUM_46)
#define BSP_I2S_WS_IO         (GPIO_NUM_9)
#define BSP_I2S_DO_IO         (GPIO_NUM_10)
#define BSP_I2S_DI_IO         (GPIO_NUM_14)
#define BSP_I2S_MCK_IO        (GPIO_NUM_3)
#define BSP_I2S_RECV_BUF_SIZE (2400)
#define BSP_I2S_SAMPLE_RATE   (44100)
#define BSP_I2S_MCLK_MULTIPLE (384)

extern i2s_chan_handle_t bsp_tx_handle;
extern i2s_chan_handle_t bsp_rx_handle;

void bsp_i2s_init(void);
void bsp_i2s_deinit(void);
void bsp_i2s_trx_start(void);
void bsp_i2s_trx_stop(void);
void bsp_i2s_set_samplerate_bits_sample(int samplerate, int bits_sample);
size_t bsp_i2s_tx_write(uint8_t *buffer, uint32_t frame_size);
size_t bsp_i2s_rx_read(uint8_t *buffer, uint32_t frame_size);

#endif // BSP_I2S_H
