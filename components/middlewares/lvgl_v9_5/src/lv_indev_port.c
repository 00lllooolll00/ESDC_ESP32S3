#include "lv_indev_port.h"

FILE_TAG("lv_indev_port.c");

static lv_indev_t *g_key_drv;
static plat_key_dev_t *g_key_dev;

void lv_port_disp_init(plat_key_dev_t *key_dev)
{
    assert(key_dev);
    g_key_dev = key_dev;
    

    
}
