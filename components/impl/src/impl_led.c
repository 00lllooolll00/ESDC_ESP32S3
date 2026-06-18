#include "impl_led.h"
#include "bsp_led.h"
#include "ek_export.h"

FILE_TAG("impl_led.c");

static int _led_dev_init(void);
static int _led_dev_deinit(void);
static int _led_dev_suspend(void);
static int _led_dev_resume(void);

static const plat_dev_ops_t s_led_base_ops = {
    .init = _led_dev_init,
    .deinit = _led_dev_deinit,
    .suspend = _led_dev_suspend,
    .resume = _led_dev_resume,
};

static int _led_set(uint8_t value);
static int _led_toggle(void);

static const plat_led_ops_t s_led_ops = {
    .set = _led_set,
    .toggle = _led_toggle,
};

// 设备实例私有于本文件，通过 impl_led_dev() 访问
static plat_led_dev_t s_led_dev;

plat_led_dev_t *impl_led_dev(void)
{
    return &s_led_dev;
}

// 设备注册：无参，供 EK_EXPORT_COMPONENTS 自动调用
static void impl_led_register(void)
{
    LOG_INFO("ek_export: COMPONENTS impl_led_register");
    plat_led_dev_register(&s_led_dev, "led", &s_led_base_ops, &s_led_ops, NULL);
}

EK_EXPORT_COMPONENTS(impl_led_register, 0);
// 设备 init：建锁 + bsp 初始化，依赖 register 完成
static void _auto_led_dev_init(void) { LOG_INFO("ek_export: COMPONENTS _auto_led_dev_init"); (void)plat_led_dev_init(&s_led_dev); }
EK_EXPORT_COMPONENTS(_auto_led_dev_init, 1);

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
