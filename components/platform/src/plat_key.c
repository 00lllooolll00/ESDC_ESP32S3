#include "plat_key.h"

void plat_key_dev_register(
    plat_key_dev_t *key, const char *name, const plat_dev_ops_t *base_ops, const plat_key_ops_t *ops, void *priv)
{
    plat_dev_register(PLAT_GET_BASE(key), name, base_ops, priv);
    key->ops = ops;
    key->base.state = PLAT_STATE_NONE;
}

int plat_key_dev_init(plat_key_dev_t *key)
{
    assert(key);
    return plat_dev_init(PLAT_GET_BASE(key));
}

int plat_key_dev_deinit(plat_key_dev_t *key)
{
    assert(key);
    return plat_dev_deinit(PLAT_GET_BASE(key));
}

int plat_key_dev_cb_register(plat_key_dev_t *key, void (*cb)(void))
{
    assert(key);
    PLAT_DEV_CHECK(key);

    key->ops->register_int_cb(cb);

    return 0;
}

int plat_key_dev_enable_int(plat_key_dev_t *key)
{
    assert(key);
    PLAT_DEV_CHECK(key);

    int err = key->ops->enable_int();

    return err;
}

int plat_key_dev_disable_int(plat_key_dev_t *key)
{
    assert(key);
    PLAT_DEV_CHECK(key);

    int err = key->ops->disable_int();

    return err;
}

int plat_key_dev_read_raw(plat_key_dev_t *key, plat_key_state_t *state)
{
    assert(key);
    PLAT_DEV_CHECK(key);
    return key->ops->read_raw(state);
}
