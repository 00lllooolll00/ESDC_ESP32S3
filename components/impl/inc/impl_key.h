#ifndef IMPL_KEY_H
#define IMPL_KEY_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "plat_key.h"

// 获取按键设备指针（设备实例私有于 impl_key.c）
plat_key_dev_t *impl_key_dev(void);

#ifdef __cplusplus
}
#endif

#endif // IMPL_KEY_H
