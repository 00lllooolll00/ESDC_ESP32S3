#ifndef IMPL_TTS_H
#define IMPL_TTS_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "plat_tts.h"

// 获取 TTS 设备指针（设备实例私有于 impl_tts.c）
plat_tts_dev_t *impl_tts_dev(void);

#ifdef __cplusplus
}
#endif

#endif // IMPL_TTS_H
