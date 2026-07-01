#include "impl_led.h"
#include "bsp_led.h"
#include "ek_export.h"

FILE_TAG("impl_led.c");

static int _led_dev_init(void);
static int _led_dev_deinit(void);
static int _led_dev_suspend(void);
static int _led_dev_resume(void);

static int _led_r_set(uint8_t value);
static int _led_g_set(uint8_t value);
static int _led_b_set(uint8_t value);
static int _led_r_toggle(void);
static int _led_g_toggle(void);
static int _led_b_toggle(void);

static const plat_dev_ops_t s_led_base_ops = {
    .init = _led_dev_init,
    .deinit = _led_dev_deinit,
    .suspend = _led_dev_suspend,
    .resume = _led_dev_resume,
};

static const plat_led_ops_t s_led_r_ops = {
    .set = _led_r_set,
    .toggle = _led_r_toggle,
};

static const plat_led_ops_t s_led_g_ops = {
    .set = _led_g_set,
    .toggle = _led_g_toggle,
};

static const plat_led_ops_t s_led_b_ops = {
    .set = _led_b_set,
    .toggle = _led_b_toggle,
};

static plat_led_dev_t s_led_r_dev;
static plat_led_dev_t s_led_g_dev;
static plat_led_dev_t s_led_b_dev;

plat_led_dev_t *impl_led_r_dev(void)
{
    return &s_led_r_dev;
}

plat_led_dev_t *impl_led_g_dev(void)
{
    return &s_led_g_dev;
}

plat_led_dev_t *impl_led_b_dev(void)
{
    return &s_led_b_dev;
}

static void impl_led_register(void)
{
    LOG_INFO("ek_export: COMPONENTS impl_led_register");
    plat_led_dev_register(&s_led_r_dev, "led_r", &s_led_base_ops, &s_led_r_ops, NULL);
    plat_led_dev_register(&s_led_g_dev, "led_g", &s_led_base_ops, &s_led_g_ops, NULL);
    plat_led_dev_register(&s_led_b_dev, "led_b", &s_led_base_ops, &s_led_b_ops, NULL);
}

EK_EXPORT_COMPONENTS(impl_led_register, 0);

static void _auto_led_dev_init(void)
{
    LOG_INFO("ek_export: COMPONENTS _auto_led_dev_init");
    plat_led_dev_init(&s_led_r_dev);
    plat_led_dev_init(&s_led_g_dev);
    plat_led_dev_init(&s_led_b_dev);
}

EK_EXPORT_COMPONENTS(_auto_led_dev_init, 1);

static int _led_dev_init(void)
{
    static bool s_led_bsp_inited = false;

    if (!s_led_bsp_inited)
    {
        bsp_led_init();
        s_led_bsp_inited = true;
    }

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

static int _led_r_set(uint8_t value)
{
    bsp_led_r_set(value);
    return 0;
}

static int _led_g_set(uint8_t value)
{
    bsp_led_g_set(value);
    return 0;
}

static int _led_b_set(uint8_t value)
{
    bsp_led_b_set(value);
    return 0;
}

static int _led_r_toggle(void)
{
    bsp_led_r_toggle();
    return 0;
}

static int _led_g_toggle(void)
{
    bsp_led_g_toggle();
    return 0;
}

static int _led_b_toggle(void)
{
    bsp_led_b_toggle();
    return 0;
}

