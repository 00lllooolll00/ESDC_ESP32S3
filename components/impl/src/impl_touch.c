#include "impl_touch.h"
#include "bsp_touch.h"

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

static int _touch_scan(plat_touch_points_t *points);
static int _touch_enable_int(void);
static int _touch_disable_int(void);
static int _touch_register_int_cb(void (*cb)(void));

static const plat_touch_ops_t s_touch_ops = {
    .scan = _touch_scan,
    .enable_int = _touch_enable_int,
    .disable_int = _touch_disable_int,
    .register_int_cb = _touch_register_int_cb,
};

static void (*s_touch_cb)(void);

int impl_touch_register(plat_touch_dev_t *dev)
{
    plat_touch_dev_register(dev, "touch", &s_touch_base_ops, &s_touch_ops, NULL);
    return 0;
}

static int _touch_dev_init(plat_dev_t *dev)
{
    bsp_touch_init();
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

static int _touch_scan(plat_touch_points_t *points)
{
    bsp_touch_points_t bsp_pts;
    bsp_touch_scan(&bsp_pts);

    uint8_t n = bsp_pts.count;
    if (n > points->capacity) n = points->capacity;
    points->count = n;
    for (int i = 0; i < n; i++)
    {
        points->points[i].x = bsp_pts.points[i].x;
        points->points[i].y = bsp_pts.points[i].y;
    }

    return 0;
}

static int _touch_enable_int(void)
{
    bsp_touch_int_enable(s_touch_cb);
    return 0;
}

static int _touch_disable_int(void)
{
    bsp_touch_int_disable();
    return 0;
}

static int _touch_register_int_cb(void (*cb)(void))
{
    s_touch_cb = cb;
    return 0;
}
