#ifndef IMPL_TOUCH_H
#define IMPL_TOUCH_H

#include "plat_touch.h"

#define IMPL_TOUCH_POINT_MAX 5

int impl_touch_register(plat_touch_dev_t *dev, uint8_t count);

#endif // IMPL_TOUCH_H
