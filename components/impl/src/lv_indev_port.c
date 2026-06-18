#include "lv_indev_port.h"
#include "plat_touch.h"
#include "impl_touch.h"
#include "ek_export.h"

FILE_TAG("lv_indev_port.c");

// tocuh
static void _plat_touch_isr_cb(void);
static void _lv_touch_read_cb(lv_indev_t *indev, lv_indev_data_t *data);

static plat_touch_dev_t *s_touch_dev;
static uint8_t s_touch_count;
static bool *s_touch_datas_flag;
static lv_indev_t **s_touchs;

void lv_port_touch_init(void)
{
    LOG_INFO("ek_export: APP lv_port_touch_init");
    plat_touch_dev_t *dev = impl_touch_dev();
    uint8_t count = IMPL_TOUCH_POINT_MAX;
    assert(dev);
    assert(count > 0);

    s_touch_datas_flag = calloc((sizeof(*s_touch_datas_flag)), count);
    assert(s_touch_datas_flag);
    s_touchs = calloc(sizeof(*s_touchs), count);
    assert(s_touchs);

    for (uint8_t i = 0; i < count; i++)
    {
        s_touchs[i] = lv_indev_create();
        lv_indev_set_type(s_touchs[i], LV_INDEV_TYPE_POINTER);
        lv_indev_set_read_cb(s_touchs[i], _lv_touch_read_cb);
        lv_indev_set_user_data(s_touchs[i], (void *)(uintptr_t)i);
    }

    s_touch_count = count;
    s_touch_dev = dev;
    plat_touch_dev_init(dev);
    plat_touch_dev_enable_int(dev);
    plat_touch_dev_cb_register(dev, _plat_touch_isr_cb);
}
// APP 层注册：依赖 lv_init、disp port（indev 需绑定已创建的 display）与 touch register
EK_EXPORT_APP(lv_port_touch_init, 2);

static void _plat_touch_isr_cb(void)
{
    int err = plat_touch_dev_read(s_touch_dev);
    if (err)
    {
        LOG_WARN("fail to read touch dev by i2c:%s", unified_strerror(err));
        return;
    }

    for (uint8_t i = 0; i < s_touch_count; i++)
    {
        plat_touch_data_t temp = { 0 };
        plat_touch_dev_get_tdata(s_touch_dev, i, &temp);

        if (temp.x <= 0 && temp.y <= 0) continue;
        if (temp.x > lv_obj_get_width(lv_screen_active()) || temp.y > lv_obj_get_height(lv_screen_active())) continue;
        s_touch_datas_flag[i] = true;
    }
}

static void _lv_touch_read_cb(lv_indev_t *indev, lv_indev_data_t *data)
{
    uint8_t idx = (uint8_t)(uintptr_t)lv_indev_get_user_data(indev);

    if (s_touch_datas_flag[idx])
    {
        data->state = LV_INDEV_STATE_PRESSED;
        data->point.x = s_touch_dev->t_data[idx].x;
        data->point.y = s_touch_dev->t_data[idx].y;
        s_touch_datas_flag[idx] = false;
    }
    else
    {
        data->state = LV_INDEV_STATE_RELEASED;
    }
}
