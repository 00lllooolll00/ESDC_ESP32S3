#include "lv_indev_port.h"
#include "ek_ringbuf.h"

FILE_TAG("lv_indev_port.c");

static lv_indev_t *s_key_drv;
static plat_key_dev_t *s_key_dev;
static volatile lv_timer_t *s_key_timer;

// key
static void _plat_port_read_cb(void);
static void _lv_port_read_cb(lv_indev_t *indev, lv_indev_data_t *data);

void lv_port_key_init(plat_key_dev_t *key_dev)
{
    assert(key_dev);
    s_key_dev = key_dev;

    plat_key_dev_enable_int(key_dev);
    plat_key_dev_cb_register(key_dev, _plat_port_read_cb);

    s_key_drv = lv_indev_create();
    assert(s_key_drv);

    s_key_timer = lv_indev_get_read_timer(s_key_drv);
    assert(s_key_timer);

    lv_indev_set_type(s_key_drv, LV_INDEV_TYPE_KEYPAD);
    lv_indev_set_read_cb(s_key_drv, _lv_port_read_cb);
}

static void _plat_port_read_cb(void)
{
    lv_timer_resume(s_key_timer);
}

static void _lv_port_read_cb(lv_indev_t *indev, lv_indev_data_t *data)
{
}

// tocuh
static void _plat_touch_isr_cb(void);
static void _lv_touch_read_cb(lv_indev_t *indev, lv_indev_data_t *data);

static plat_touch_dev_t *s_touch_dev;
static uint8_t s_touch_count;
static ek_ringbuf_spsc_t **s_touch_rb_spsc;
static lv_indev_t **s_touchs;

void lv_port_touch_init(plat_touch_dev_t *dev, uint8_t count)
{
    assert(dev);
    assert(count > 0);

    s_touch_rb_spsc = calloc((sizeof(*s_touch_rb_spsc)), count);
    assert(s_touch_rb_spsc);

    for (uint8_t i = 0; i < count; i++)
    {
        s_touch_rb_spsc[i] = ek_ringbuf_create_spsc(sizeof(plat_touch_data_t), 5);
        assert(s_touch_rb_spsc[i]);
    }

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

        ek_ringbuf_write_spsc(s_touch_rb_spsc[i], &temp);
    }
}

static void _lv_touch_read_cb(lv_indev_t *indev, lv_indev_data_t *data)
{
    uint8_t idx = (uint8_t)(uintptr_t)lv_indev_get_user_data(indev);
    if (!ek_ringbuf_empty_spsc(s_touch_rb_spsc[idx]))
    {
        plat_touch_data_t temp = { 0 };
        ek_ringbuf_read_spsc(s_touch_rb_spsc[idx], &temp);
        data->state = LV_INDEV_STATE_PRESSED;
        data->point.x = temp.x;
        data->point.y = temp.y;

        if (!ek_ringbuf_empty_spsc(s_touch_rb_spsc[idx])) data->continue_reading = true;
        LOG_INFO("x:%d,y:%d", temp.x, temp.y);
    }
    else
    {
        data->state = LV_INDEV_STATE_RELEASED;
    }
}
