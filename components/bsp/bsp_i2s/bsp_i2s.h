#ifndef BSP_I2S_H
#define BSP_I2S_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "common_header.h"
#include "driver/gpio.h"
#include "driver/i2s_std.h"

#define BSP_I2S               (I2S_NUM_0)
#define BSP_I2S_BCK_PIN       (GPIO_NUM_12)
#define BSP_I2S_WS_PIN        (GPIO_NUM_13)
#define BSP_I2S_DO_PIN        (GPIO_NUM_1)
#define BSP_I2S_DI_PIN        (GPIO_NUM_2)
#define BSP_I2S_MCK_PIN       (GPIO_NUM_11)

#define BSP_I2S_RECV_BUF_SIZE (2400)
#define BSP_I2S_SAMPLE_RATE   (16000)
#define BSP_I2S_MCLK_MULTIPLE (256)

void bsp_i2s_init(void);
void bsp_i2s_deinit(void);
void bsp_i2s_trx_start(void);
void bsp_i2s_trx_stop(void);
void bsp_i2s_set_samplerate_bits_sample(int samplerate, int bits_sample);
size_t bsp_i2s_tx_write(uint8_t *buffer, uint32_t frame_size);
size_t bsp_i2s_rx_read(uint8_t *buffer, uint32_t frame_size);

#ifdef __cplusplus
}
#endif

#endif // BSP_I2S_H
