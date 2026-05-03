#include "impl_key.h"
#include "bsp_key.h"

static int _key_dev_init(plat_dev_t *dev);
static int _key_dev_deinit(plat_dev_t *dev);
static int _key_dev_suspend(plat_dev_t *dev);
static int _key_dev_resume(plat_dev_t *dev);

static const plat_dev_ops_t s_key_base_ops = {
    .init = _key_dev_init,
    .deinit = _key_dev_deinit,
    .suspend = _key_dev_suspend,
    .resume = _key_dev_resume,
};

static int _key_read_raw(plat_key_state_t *state);
static int _key_enable_int(void);
static int _key_disable_int(void);
static int _key_register_int_cb(void (*cb)(void));

static const plat_key_ops_t s_key_ops = {
    .read_raw = _key_read_raw,
    .enable_int = _key_enable_int,
    .disable_int = _key_disable_int,
    .register_int_cb = _key_register_int_cb,
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

static int _key_read_raw(plat_key_state_t *state)
{
    *state = (plat_key_state_t)bsp_key_read_raw();
    return 0;
}

static int _key_enable_int(void)
{
    bsp_exio_int_enable();
    return 0;
}

static int _key_disable_int(void)
{
    bsp_exio_int_disable();
    return 0;
}

static int _key_register_int_cb(void (*cb)(void))
{
    bsp_exio_set_int_cb(cb);
    return 0;
}
