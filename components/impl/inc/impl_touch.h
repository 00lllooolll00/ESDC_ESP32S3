#ifndef IMPL_TOUCH_H
#define IMPL_TOUCH_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "plat_touch.h"

#define IMPL_TOUCH_POINT_MAX 5

// 获取触摸设备指针（设备实例私有于 impl_touch.c）
plat_touch_dev_t *impl_touch_dev(void);

#ifdef __cplusplus
}
#endif

#endif // IMPL_TOUCH_H
