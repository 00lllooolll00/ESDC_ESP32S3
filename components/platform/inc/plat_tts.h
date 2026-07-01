#ifndef PLAT_TTS_H
#define PLAT_TTS_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "plat_base.h"

typedef struct plat_tts_ops_t plat_tts_ops_t;
typedef struct plat_tts_dev_t plat_tts_dev_t;

/* TTS 设备特有操作 */
struct plat_tts_ops_t
{
    int (*speak)(const char *text); /* 播放中文文本（阻塞直至播完）*/
    int (*enable_amp)(int enable); /* 功放使能：1=使能(开), 0=关闭(静音) */
};

struct plat_tts_dev_t
{
    plat_dev_t base;
    const plat_tts_ops_t *ops;
};

void plat_tts_dev_register(
    plat_tts_dev_t *tts, const char *name, const plat_dev_ops_t *base_ops, const plat_tts_ops_t *ops, void *priv);
int plat_tts_dev_init(plat_tts_dev_t *tts);
int plat_tts_dev_deinit(plat_tts_dev_t *tts);
int plat_tts_dev_speak(plat_tts_dev_t *tts, const char *text);
int plat_tts_dev_enable_amp(plat_tts_dev_t *tts, int enable);

#ifdef __cplusplus
}
#endif

#endif // PLAT_TTS_H
