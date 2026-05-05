#include "impl_touch.h"
#include "bsp_touch.h"

typedef struct impl_touch_point_t
{
    plat_touch_dev_t dev;
    plat_touch_data_t cache;
    void (*cb)(void);
} impl_touch_point_t;

static int _touch_dev_init(plat_dev_t *dev);
static int _touch_dev_deinit(plat_dev_t *dev);
static int _touch_dev_suspend(plat_dev_t *dev);
static int _touch_dev_resume(plat_dev_t *dev);

static const plat_dev_ops_t s_touch_base_ops = {
    .init = _touch_dev_init,
    .deinit = _touch_dev_deinit,
    .suspend = _touch_dev_suspend,
    .resume = _touch_dev_resume,
};

static int _touch_read(plat_touch_dev_t *dev, plat_touch_data_t *data);
static int _touch_enable_int(plat_touch_dev_t *dev);
static int _touch_disable_int(plat_touch_dev_t *dev);
static int _touch_register_int_cb(plat_touch_dev_t *dev, void (*cb)(void));

static const plat_touch_ops_t s_touch_ops = {
    .read = _touch_read,
    .enable_int = _touch_enable_int,
    .disable_int = _touch_disable_int,
    .register_int_cb = _touch_register_int_cb,
};

static impl_touch_point_t s_pool[IMPL_TOUCH_POINT_MAX];
static bool s_int_enabled;

static void _touch_global_isr_cb(void)
{
    bsp_touch_points_t bsp_pts;
    bsp_touch_scan(&bsp_pts);

    for (int i = 0; i < IMPL_TOUCH_POINT_MAX; i++)
    {
        s_pool[i].cache.pressed = false;
    }

    for (int i = 0; i < bsp_pts.count && i < IMPL_TOUCH_POINT_MAX; i++)
    {
        s_pool[i].cache.x = bsp_pts.points[i].x;
        s_pool[i].cache.y = bsp_pts.points[i].y;
        s_pool[i].cache.pressed = true;
    }

    for (int i = 0; i < IMPL_TOUCH_POINT_MAX; i++)
    {
        if (s_pool[i].cb) s_pool[i].cb();
    }
}

int impl_touch_init(void)
{
    bsp_touch_init();
    memset(s_pool, 0, sizeof(s_pool));
    s_int_enabled = false;
    return 0;
}

plat_touch_dev_t *impl_touch_point_create(uint8_t id)
{
    assert(id < IMPL_TOUCH_POINT_MAX);

    impl_touch_point_t *point = &s_pool[id];
    plat_touch_dev_register(&point->dev, "touch", &s_touch_base_ops, &s_touch_ops, NULL);

    int err = plat_touch_dev_init(&point->dev);
    if (err != 0) return NULL;

    return &point->dev;
}

static int _touch_dev_init(plat_dev_t *dev)
{
    return 0;
}

static int _touch_dev_deinit(plat_dev_t *dev)
{
    return 0;
}

static int _touch_dev_suspend(plat_dev_t *dev)
{
    return 0;
}

static int _touch_dev_resume(plat_dev_t *dev)
{
    return 0;
}

static int _touch_read(plat_touch_dev_t *dev, plat_touch_data_t *data)
{
    impl_touch_point_t *point = (impl_touch_point_t *)dev;
    *data = point->cache;
    return 0;
}

static int _touch_enable_int(plat_touch_dev_t *dev)
{
    if (!s_int_enabled)
    {
        bsp_touch_int_enable(_touch_global_isr_cb);
        s_int_enabled = true;
    }
    return 0;
}

static int _touch_disable_int(plat_touch_dev_t *dev)
{
    if (s_int_enabled)
    {
        bsp_touch_int_disable();
        s_int_enabled = false;
    }
    return 0;
}

static int _touch_register_int_cb(plat_touch_dev_t *dev, void (*cb)(void))
{
    impl_touch_point_t *point = (impl_touch_point_t *)dev;
    point->cb = cb;
    return 0;
}
