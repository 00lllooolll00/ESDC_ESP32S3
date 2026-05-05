#ifndef PLAT_TOUCH_H
#define PLAT_TOUCH_H

#include "plat_base.h"

typedef struct plat_touch_ops_t plat_touch_ops_t;
typedef struct plat_touch_dev_t plat_touch_dev_t;

typedef struct
{
    uint16_t x;
    uint16_t y;
    bool pressed;
} plat_touch_data_t;

struct plat_touch_ops_t
{
    int (*read)(plat_touch_dev_t *dev, plat_touch_data_t *data);
    int (*register_int_cb)(plat_touch_dev_t *dev, void (*cb)(void));
    int (*enable_int)(plat_touch_dev_t *dev);
    int (*disable_int)(plat_touch_dev_t *dev);
};

struct plat_touch_dev_t
{
    plat_dev_t base;
    const plat_touch_ops_t *ops;
    void (*cb)(void);
};

void plat_touch_dev_register(
    plat_touch_dev_t *dev, const char *name, const plat_dev_ops_t *base_ops, const plat_touch_ops_t *ops, void *priv);
int plat_touch_dev_init(plat_touch_dev_t *dev);
int plat_touch_dev_deinit(plat_touch_dev_t *dev);
int plat_touch_dev_read(plat_touch_dev_t *dev, plat_touch_data_t *data);
int plat_touch_dev_cb_register(plat_touch_dev_t *dev, void (*cb)(void));
int plat_touch_dev_enable_int(plat_touch_dev_t *dev);
int plat_touch_dev_disable_int(plat_touch_dev_t *dev);

#endif // PLAT_TOUCH_H
