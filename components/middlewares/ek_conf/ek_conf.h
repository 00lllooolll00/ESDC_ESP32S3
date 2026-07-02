#ifndef EK_CONF_H
#define EK_CONF_H

#include <stdio.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define EKCFG_RTOS        (0)
#define EKCFG_PICOLIBC    (0)
#define EKCFG_IO_LWPRTF   (0)
#define EKCFG_EXPORT      (1)

#define EKCFG_ASSERT      (1)
#define EKCFG_ASSERT_TINY (0)
#define EKCFG_ASSERT_LOG  (1)

// EKCFG_LOG: 对应 bak EK_LOG_ENABLE=0
#define EKCFG_LOG       (1)
#define EKCFG_LOG_COLOR (1)
#define EKCFG_LOG_DEBUG (1)

// 数据结构开关
// 对应 bak 各 EK_*_ENABLE 宏
#define EKCFG_STR          (0)
#define EKCFG_LIST         (0)
#define EKCFG_VEC          (0)
#define EKCFG_RINGBUF      (1)
#define EKCFG_RINGBUF_SPSC (1)
#define EKCFG_STACK        (1)
#define EKCFG_EVOKE        (0)

#define EKCFG_HEAP_TLSF    (0)
#define EKCFG_HEAP_SIZE    (0)

#define ek_malloc          malloc
#define ek_free            free
#define ek_realloc         realloc

#define ek_printf          printf
#define ek_vsprintf        vsprintf
#define ek_sprintf         sprintf
#define ek_snprintf        snprintf
#define ek_vsnprintf       vsnprintf

#ifdef __cplusplus
}
#endif

#endif // EK_CONF_H
