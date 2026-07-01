#include "impl_key.h"
#include "bsp_key.h"
#include "ek_export.h"

EK_LOG_FILE_TAG("impl_key.c");

static int _key_dev_init(void);
static int _key_dev_deinit(void);
static int _key_dev_suspend(void);
static int _key_dev_resume(void);

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

// 设备实例私有于本文件，通过 impl_key_dev() 访问
static plat_key_dev_t s_key_dev;

plat_key_dev_t *impl_key_dev(void)
{
    return &s_key_dev;
}

// 设备注册：无参，供 EK_EXPORT_COMPONENTS 自动调用
static void impl_key_register(void)
{
    EK_LOG_INFO("ek_export: COMPONENTS impl_key_register");
    plat_key_dev_register(&s_key_dev, "key", &s_key_base_ops, &s_key_ops, NULL);
}

EK_EXPORT_COMPONENTS(impl_key_register, 0);
// 设备 init：建锁 + bsp 初始化，依赖 register 完成
static void _auto_key_dev_init(void)
{
    EK_LOG_INFO("ek_export: COMPONENTS _auto_key_dev_init");
    (void)plat_key_dev_init(&s_key_dev);
}
EK_EXPORT_COMPONENTS(_auto_key_dev_init, 1);

static int _key_dev_init(void)
{
    bsp_key_init();
    return 0;
}

static int _key_dev_deinit(void)
{
    return 0;
}

static int _key_dev_suspend(void)
{
    return 0;
}

static int _key_dev_resume(void)
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
