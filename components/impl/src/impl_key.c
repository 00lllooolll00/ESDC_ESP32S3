#include "impl_key.h"
#include "bsp_key.h"

static int _key_dev_init(plat_dev_t *dev);
static int _key_dev_deinit(plat_dev_t *dev);
static int _key_dev_suspend(plat_dev_t *dev);
static int _key_dev_resume(plat_dev_t *dev);

static const plat_dev_ops_t s_key_base_ops = {
    .init    = _key_dev_init,
    .deinit  = _key_dev_deinit,
    .suspend = _key_dev_suspend,
    .resume  = _key_dev_resume,
};

static int _key_read_raw(plat_key_dev_t *key, plat_key_state_t *state);

static const plat_key_ops_t s_key_ops = {
    .read_raw = _key_read_raw,
};

int impl_key_register(plat_key_dev_t *key_dev)
{
    plat_key_dev_register(key_dev, "key", &s_key_base_ops, &s_key_ops, NULL);
    return 0;
}

static int _key_dev_init(plat_dev_t *dev)
{
    bsp_key_init();
    return 0;
}

static int _key_dev_deinit(plat_dev_t *dev)
{
    return 0;
}

static int _key_dev_suspend(plat_dev_t *dev)
{
    return 0;
}

static int _key_dev_resume(plat_dev_t *dev)
{
    return 0;
}

static int _key_read_raw(plat_key_dev_t *key, plat_key_state_t *state)
{
    *state = (plat_key_state_t)bsp_key_read_raw();
    return 0;
}