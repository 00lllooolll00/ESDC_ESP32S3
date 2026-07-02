#include "impl_touch.h"
#include "bsp_touch.h"
#include "ek_export.h"

EK_LOG_FILE_TAG("impl_touch.c");

static int _touch_dev_init(void);
static int _touch_dev_deinit(void);
static int _touch_dev_suspend(void);
static int _touch_dev_resume(void);

static const plat_dev_ops_t s_touch_base_ops = {
    .init = _touch_dev_init,
    .deinit = _touch_dev_deinit,
    .suspend = _touch_dev_suspend,
    .resume = _touch_dev_resume,
};

static int _touch_read(plat_touch_dev_t *dev);
static int _get_tdata(uint8_t idx, plat_touch_data_t *tdata);
static int _touch_enable_int(void);
static int _touch_disable_int(void);
static int _touch_register_int_cb(void (*cb)(void));

static const plat_touch_ops_t s_touch_ops = {
    .read = _touch_read,
    .get_tdata = _get_tdata,
    .enable_int = _touch_enable_int,
    .disable_int = _touch_disable_int,
    .register_int_cb = _touch_register_int_cb,
};

static bsp_touch_points_t s_tps;

// 设备实例私有于本文件，通过 impl_touch_dev() 访问
static plat_touch_dev_t s_touch_dev;

plat_touch_dev_t *impl_touch_dev(void)
{
    return &s_touch_dev;
}

// 设备注册：无参，count 取 IMPL_TOUCH_POINT_MAX，供 EK_EXPORT_COMPONENTS 自动调用
static void impl_touch_register(void)
{
    EK_LOG_INFO("ek_export: COMPONENTS impl_touch_register");
    plat_touch_dev_register(&s_touch_dev, "touch", &s_touch_base_ops, &s_touch_ops, IMPL_TOUCH_POINT_MAX, NULL);
}

EK_EXPORT_COMPONENTS(impl_touch_register, 0);

static int _touch_dev_init(void)
{
    bsp_touch_init();
    return 0;
}

static int _touch_dev_deinit(void)
{
    return 0;
}

static int _touch_dev_suspend(void)
{
    return 0;
}

static int _touch_dev_resume(void)
{
    return 0;
}

static int _touch_read(plat_touch_dev_t *dev)
{
    int err = bsp_touch_read(&s_tps);

    if (!err)
    {
        for (uint8_t i = 0; i < dev->t_count; i++)
        {
            dev->t_data[i].x = s_tps.points[i].x;
            dev->t_data[i].y = s_tps.points[i].y;
        }
    }
    return err;
}

static int _touch_enable_int(void)
{
    bsp_touch_int_enable();
    return 0;
}

static int _touch_disable_int()
{
    bsp_touch_int_disable();
    return 0;
}

static int _touch_register_int_cb(void (*cb)(void))
{
    bsp_touch_register_int_cb(cb);
    return 0;
}

static int _get_tdata(uint8_t idx, plat_touch_data_t *tdata)
{
    assert(idx < BSP_TOUCH_POINT_NUM);

    tdata->x = s_tps.points[idx].x;
    tdata->y = s_tps.points[idx].y;
    return 0;
}
