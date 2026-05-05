#include "plat_touch.h"

void plat_touch_dev_register(plat_touch_dev_t *dev,
                             const char *name,
                             const plat_dev_ops_t *base_ops,
                             const plat_touch_ops_t *ops,
                             uint8_t count,
                             void *priv)
{
    plat_dev_register(PLAT_GET_BASE(dev), name, base_ops, priv);
    dev->ops = ops;
    dev->t_count = count;
    dev->base.state = PLAT_STATE_NONE;
    dev->t_data = calloc(count, sizeof(plat_touch_data_t));
    assert(dev->t_data);
}

int plat_touch_dev_init(plat_touch_dev_t *dev)
{
    assert(dev);
    return plat_dev_init(PLAT_GET_BASE(dev));
}

int plat_touch_dev_deinit(plat_touch_dev_t *dev)
{
    assert(dev);
    return plat_dev_deinit(PLAT_GET_BASE(dev));
}

int plat_touch_dev_read(plat_touch_dev_t *dev)
{
    assert(dev);
    PLAT_DEV_CHECK(dev);
    return dev->ops->read(dev);
}

int plat_touch_dev_cb_register(plat_touch_dev_t *dev, void (*cb)(void))
{
    assert(dev);
    PLAT_DEV_CHECK(dev);

    dev->ops->register_int_cb(cb);

    return 0;
}

int plat_touch_dev_enable_int(plat_touch_dev_t *dev)
{
    assert(dev);
    PLAT_DEV_CHECK(dev);

    int err = dev->ops->enable_int();

    return err;
}

int plat_touch_dev_disable_int(plat_touch_dev_t *dev)
{
    assert(dev);
    PLAT_DEV_CHECK(dev);

    int err = dev->ops->disable_int();

    return err;
}

int plat_touch_dev_get_tdata(plat_touch_dev_t *dev, uint8_t idx, plat_touch_data_t *tdata)
{
    assert(dev);
    PLAT_DEV_CHECK(dev);

    if (idx > dev->t_count) return EINVAL;

    *tdata = dev->t_data[idx];

    return 0;
}
