#include "bsp_audio.h"
#include "bsp_i2c.h"

EK_LOG_FILE_TAG("bsp_audio.c");

static esp_err_t _write_reg(uint8_t reg_addr, uint8_t data);
static esp_err_t _read_reg(uint8_t reg_addr, uint8_t *pdata);

static i2c_master_dev_handle_t s_audio_i2c_handle;

int bsp_audio_init(void)
{
    bsp_i2c_init(I2C_NUM_0);
    bsp_i2c_dev_register(I2C_NUM_0, EK_FREQ_K(400), BSP_AUDIO_ADDR, &s_audio_i2c_handle);

    uint8_t temp = 0;
    temp |= _write_reg(0, 0x80); /* 软复位ES8388 */
    temp |= _write_reg(0, 0x00);
    vTaskDelay(pdMS_TO_TICKS(200)); /* 等待复位 */

    temp |= _write_reg(0x01, 0x58);
    temp |= _write_reg(0x01, 0x50);
    temp |= _write_reg(0x02, 0xF3);
    temp |= _write_reg(0x02, 0xF0);

    temp |= _write_reg(0x03, 0x09); /* 麦克风偏置电源关闭 */
    temp |= _write_reg(0x00, 0x06); /* 使能参考 500K驱动使能 */
    temp |= _write_reg(0x04, 0x00); /* DAC电源管理，不打开任何通道 */
    temp |= _write_reg(0x08, 0x00); /* MCLK不分频 */
    temp |= _write_reg(0x2B, 0x80); /* DAC控制 DACLRC与ADCLRC相同 */

    temp |= _write_reg(0x09, 0x88); /* ADC L/R PGA增益配置为+24dB */
    temp |= _write_reg(0x0C, 0x4C); /* ADC数据选择为left data = left ADC, right data = left ADC  音频数据为16bit */
    temp |= _write_reg(0x0D, 0x02); /* ADC配置 MCLK/采样率=256 */
    temp |= _write_reg(0x10, 0x00); /* ADC数字音量控制将信号衰减 L  设置为最小！！！ */
    temp |= _write_reg(0x11, 0x00); /* ADC数字音量控制将信号衰减 R  设置为最小！！！ */

    temp |= _write_reg(0x17, 0x18); /* DAC音频数据为16bit */
    temp |= _write_reg(0x18, 0x02); /* DAC配置 MCLK/采样率=256 */
    temp |= _write_reg(0x1A, 0x00); /* DAC数字音量控制将信号衰减 L  设置为最小！！！ */
    temp |= _write_reg(0x1B, 0x00); /* DAC数字音量控制将信号衰减 R  设置为最小！！！ */
    temp |= _write_reg(0x27, 0xB8); /* L混频器 */
    temp |= _write_reg(0x2A, 0xB8); /* R混频器 */
    vTaskDelay(pdMS_TO_TICKS(100));

    if (temp != ESP_OK)
    {
        EK_LOG_ERROR("fail to init audio");
        return 1;
    }
    else
    {
        EK_LOG_INFO("successfully init the audio");
        bsp_audio_adda_cfg(1, 0); /* 开启DAC，关闭ADC（仅需播放）*/
        bsp_audio_input_cfg(0); /* 关闭录音输入 */
        bsp_audio_output_cfg(1, 1); /* 开启输出通道1、2 */
        bsp_audio_spkvol_set(30); /* 喇叭音量（较低，0~33）*/
        vTaskDelay(pdMS_TO_TICKS(100));
        return 0;
    }
}

/**
 * @brief       ES8388反初始化
 * @param       无
 * @retval      0,初始化正常
 *              其他,错误代码
 */
esp_err_t bsp_audio_deinit(void)
{
    return _write_reg(0x02, 0xFF); /* 复位和暂停ES8388 */
}

/**
 * @brief       设置ES8388工作模式
 * @param       fmt : 工作模式
 *    @arg      0, 飞利浦标准I2S;
 *    @arg      1, MSB(左对齐);
 *    @arg      2, LSB(右对齐);
 *    @arg      3, PCM/DSP
 * @param       len : 数据长度
 *    @arg      0, 24bit
 *    @arg      1, 20bit
 *    @arg      2, 18bit
 *    @arg      3, 16bit
 *    @arg      4, 32bit
 * @retval      无
 */
void bsp_audio_i2s_cfg(uint8_t fmt, uint8_t len)
{
    fmt &= 0x03;
    len &= 0x07; /* 限定范围 */
    _write_reg(23, (fmt << 1) | (len << 3)); /* R23,ES8388工作模式设置 */
}

/**
 * @brief       设置耳机音量
 * @param       volume : 音量大小(0 ~ 33)
 * @retval      无
 */
void bsp_audio_hpvol_set(uint8_t volume)
{
    if (volume > 33)
    {
        volume = 33;
    }

    _write_reg(0x2E, volume);
    _write_reg(0x2F, volume);
}

/**
 * @brief       设置喇叭音量
 * @param       volume : 音量大小(0 ~ 33)
 * @retval      无
 */
void bsp_audio_spkvol_set(uint8_t volume)
{
    if (volume > 33)
    {
        volume = 33;
    }

    _write_reg(0x30, volume);
    _write_reg(0x31, volume);
}

/**
 * @brief       设置3D环绕声
 * @param       depth : 0 ~ 7(3D强度,0关闭,7最强)
 * @retval      无
 */
void bsp_audio_3d_set(uint8_t depth)
{
    depth &= 0x7; /* 限定范围 */
    _write_reg(0x1D, depth << 2); /* R7,3D环绕设置 */
}

/**
 * @brief       ES8388 DAC/ADC配置
 * @param       dacen : dac使能(1) / 关闭(0)
 * @param       adcen : adc使能(1) / 关闭(0)
 * @retval      无
 */
void bsp_audio_adda_cfg(uint8_t dacen, uint8_t adcen)
{
    uint8_t tempreg = 0;

    tempreg |= !dacen << 0;
    tempreg |= !adcen << 1;
    tempreg |= !dacen << 2;
    tempreg |= !adcen << 3;
    _write_reg(0x02, tempreg);
}

/**
 * @brief       ES8388 DAC输出通道配置
 * @param       o1en : 通道1使能(1)/禁止(0)
 * @param       o2en : 通道2使能(1)/禁止(0)
 * @retval      无
 */
void bsp_audio_output_cfg(uint8_t o1en, uint8_t o2en)
{
    uint8_t tempreg = 0;
    tempreg |= o1en * (3 << 4);
    tempreg |= o2en * (3 << 2);
    _write_reg(0x04, tempreg);
}

/**
 * @brief       ES8388 MIC增益设置(MIC PGA增益)
 * @param       gain : 0~8, 对应0~24dB  3dB/Step
 * @retval      无
 */
void bsp_audio_mic_gain(uint8_t gain)
{
    gain &= 0x0F;
    gain |= gain << 4;
    _write_reg(0x09, gain); /* R9,左右通道PGA增益设置 */
}

/**
 * @brief       ES8388 ALC设置
 * @param       sel
 *   @arg       0,关闭ALC
 *   @arg       1,右通道ALC
 *   @arg       2,左通道ALC
 *   @arg       3,立体声ALC
 * @param       maxgain : 0~7,对应-6.5~+35.5dB
 * @param       minigain: 0~7,对应-12~+30dB 6dB/STEP
 * @retval      无
 */
void bsp_audio_alc_ctrl(uint8_t sel, uint8_t maxgain, uint8_t mingain)
{
    uint8_t tempreg = 0;
    tempreg = sel << 6;
    tempreg |= (maxgain & 0x07) << 3;
    tempreg |= mingain & 0x07;
    _write_reg(0x12, tempreg); /* R18,ALC设置 */
}

/**
 * @brief       ES8388 ADC输出通道配置
 * @param       in : 输入通道
 *    @arg      0, 通道1输入
 *    @arg      1, 通道2输入
 * @retval      无
 */
void bsp_audio_input_cfg(uint8_t in)
{
    _write_reg(0x0A, (5 * in) << 4); /* ADC1 输入通道选择L/R INPUT1 */
}

static esp_err_t _write_reg(uint8_t reg_addr, uint8_t data)
{
    return bsp_i2c_mem_write(s_audio_i2c_handle, reg_addr, &data, 1, BSP_AUDIO_TIMEOUT);
}

static esp_err_t _read_reg(uint8_t reg_addr, uint8_t *pdata)
{
    return bsp_i2c_mem_read(s_audio_i2c_handle, reg_addr, pdata, 1, BSP_AUDIO_TIMEOUT);
}
