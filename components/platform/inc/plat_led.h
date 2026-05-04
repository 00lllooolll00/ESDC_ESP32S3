#ifndef PLAT_LED_H
#define PLAT_LED_H

#include "plat_base.h"

typedef struct plat_led_ops_t plat_led_ops_t;
typedef struct plat_led_dev_t plat_led_dev_t;

struct plat_led_ops_t {
    int (*set)(uint8_t value);
    int (*toggle)(void);
};

struct plat_led_dev_t {
    plat_dev_t base;
    const plat_led_ops_t *ops;
};

void plat_led_dev_register(plat_led_dev_t *led, const char *name,
                           const plat_dev_ops_t *base_ops, const plat_led_ops_t *ops, void *priv);
int plat_led_dev_init(plat_led_dev_t *led);
int plat_led_dev_deinit(plat_led_dev_t *led);
int plat_led_dev_set(plat_led_dev_t *led, uint8_t value);
int plat_led_dev_toggle(plat_led_dev_t *led);

#endif // PLAT_LED_H
