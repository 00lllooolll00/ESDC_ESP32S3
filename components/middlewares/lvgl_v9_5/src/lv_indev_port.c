#include "lv_indev_port.h"

FILE_TAG("lv_indev_port.c");

static lv_indev_t *s_key_drv;
static plat_key_dev_t *s_key_dev;
static volatile lv_timer_t *s_key_timer;

static void _plat_port_read_cb(void);
static void _lv_port_read_cb(lv_indev_t *indev, lv_indev_data_t *data);

void lv_port_disp_init(plat_key_dev_t *key_dev)
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
