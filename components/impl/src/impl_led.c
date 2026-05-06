#include "impl_led.h"
#include "bsp_led.h"

static int _led_dev_init(void);
static int _led_dev_deinit(void);
static int _led_dev_suspend(void);
static int _led_dev_resume(void);

static const plat_dev_ops_t s_led_base_ops = {
    .init    = _led_dev_init,
    .deinit  = _led_dev_deinit,
    .suspend = _led_dev_suspend,
    .resume  = _led_dev_resume,
};

static int _led_set(uint8_t value);
static int _led_toggle(void);

static const plat_led_ops_t s_led_ops = {
    .set    = _led_set,
    .toggle = _led_toggle,
};

int impl_led_register(plat_led_dev_t *led_dev)
{
    plat_led_dev_register(led_dev, "led", &s_led_base_ops, &s_led_ops, NULL);
    return 0;
}

static int _led_dev_init(void)
{
    bsp_led_init();
    return 0;
}

static int _led_dev_deinit(void)
{
    return 0;
}

static int _led_dev_suspend(void)
{
    return 0;
}

static int _led_dev_resume(void)
{
    return 0;
}

static int _led_set(uint8_t value)
{
    bsp_led_set(value);
    return 0;
}

static int _led_toggle(void)
{
    bsp_led_toggle();
    return 0;
}
