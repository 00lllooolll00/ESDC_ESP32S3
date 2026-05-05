#ifndef IMPL_TOUCH_H
#define IMPL_TOUCH_H

#include "plat_touch.h"

#define IMPL_TOUCH_POINT_MAX 5

int impl_touch_init(void);
plat_touch_dev_t *impl_touch_point_create(uint8_t id);

#endif // IMPL_TOUCH_H
