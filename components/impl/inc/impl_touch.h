#ifndef IMPL_TOUCH_H
#define IMPL_TOUCH_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "plat_touch.h"

#define IMPL_TOUCH_POINT_MAX 5

int impl_touch_register(plat_touch_dev_t *dev, uint8_t count);

#ifdef __cplusplus
}
#endif

#endif // IMPL_TOUCH_H
