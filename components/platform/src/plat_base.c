#include "plat_base.h"

void plat_dev_register(plat_dev_t *dev, const char *name, const plat_dev_ops_t *base_ops, void *priv)
{
    dev->name = name;
    dev->base_ops = base_ops;
    dev->priv = priv;
}

int plat_dev_init(plat_dev_t *dev)
{
    assert(dev);
    if (dev->state != PLAT_STATE_NONE) return 0;

    dev->lock = xSemaphoreCreateMutex();

    if (!dev->lock) return ENOMEM;

    int err = dev->base_ops->init();

    if (err) dev->state = PLAT_STATE_ERROR;
    else dev->state = PLAT_STATE_READY;

    return err;
}

int plat_dev_deinit(plat_dev_t *dev)
{
    assert(dev);
    if (dev->state != PLAT_STATE_READY && dev->state != PLAT_STATE_ERROR) return EBUSY;

    vSemaphoreDelete(dev->lock);

    int err = dev->base_ops->deinit();

    if (err) dev->state = PLAT_STATE_ERROR;
    else dev->state = PLAT_STATE_NONE;

    return err;
}

int plat_dev_suspend(plat_dev_t *dev)
{
    assert(dev);

    if (dev->state != PLAT_STATE_READY) return EBUSY;

    dev->state = PLAT_STATE_SUSPEND;

    return 0;
}

int plat_dev_resume(plat_dev_t *dev)
{
    assert(dev);

    if (dev->state != PLAT_STATE_SUSPEND) return 0;

    dev->state = PLAT_STATE_READY;

    return 0;
}
