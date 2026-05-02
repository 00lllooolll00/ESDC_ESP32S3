#ifndef BSP_AUDIO_H
#define BSP_AUDIO_H

#include "common_header.h"
#include "driver/gpio.h"

#define BSP_AUDIO_ADDR    0x10
#define BSP_AUDIO_TIMEOUT 100
// i2s
#define BSP_AUDIO_MCLK  GPIO_NUM_3
#define BSP_AUDIO_SCK   GPIO_NUM_46
#define BSP_AUDIO_LRCK  GPIO_NUM_9
#define BSP_AUDIO_SDIN  GPIO_NUM_10
#define BSP_AUDIO_SDOUT GPIO_NUM_14

// iic
#define BSP_AUDIO_IIC_SCL GPIO_NUM_42
#define BSP_AUDIO_IIC_SDA GPIO_NUM_41

int bsp_audio_init(void);
esp_err_t bsp_audio_deinit(void);
void bsp_audio_i2s_cfg(uint8_t fmt, uint8_t len);
void bsp_audio_hpvol_set(uint8_t volume);
void bsp_audio_spkvol_set(uint8_t volume);
void bsp_audio_3d_set(uint8_t depth);
void bsp_audio_adda_cfg(uint8_t dacen, uint8_t adcen);
void bsp_audio_output_cfg(uint8_t o1en, uint8_t o2en);
void bsp_audio_mic_gain(uint8_t gain);
void bsp_audio_alc_ctrl(uint8_t sel, uint8_t maxgain, uint8_t mingain);
void bsp_audio_input_cfg(uint8_t in);

#endif // BSP_AUDIO_H
