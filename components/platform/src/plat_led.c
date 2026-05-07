#include "plat_led.h"

void plat_led_dev_register(
    plat_led_dev_t *led, const char *name, const plat_dev_ops_t *base_ops, const plat_led_ops_t *ops, void *priv)
{
    plat_dev_register(PLAT_GET_BASE(led), name, base_ops, priv);
    led->ops = ops;
}

int plat_led_dev_init(plat_led_dev_t *led)
{
    assert(led);
    return plat_dev_init(PLAT_GET_BASE(led));
}

int plat_led_dev_deinit(plat_led_dev_t *led)
{
    assert(led);
    return plat_dev_deinit(PLAT_GET_BASE(led));
}

int plat_led_dev_set(plat_led_dev_t *led, uint8_t value)
{
    PLAT_DEV_CHECK(led);
    return led->ops->set(value);
}

int plat_led_dev_toggle(plat_led_dev_t *led)
{
    PLAT_DEV_CHECK(led);
    return led->ops->toggle();
}
