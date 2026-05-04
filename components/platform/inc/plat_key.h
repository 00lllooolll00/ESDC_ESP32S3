#ifndef PLAT_KEY_H
#define PLAT_KEY_H

#include "plat_base.h"

typedef struct plat_key_ops_t plat_key_ops_t;
typedef struct plat_key_dev_t plat_key_dev_t;

typedef enum
{
    PLAT_KEY_STATE_NONE = 0,
    PLAT_KEY_STATE_0 = 0x01,
    PLAT_KEY_STATE_1 = 0x02,
    PLAT_KEY_STATE_2 = 0x04,
    PLAT_KEY_STATE_3 = 0x08,
    PLAT_KEY_STATE_BOOT = 0x10,
    PLAT_KEY_STATE_MAX = 0x1F,
} plat_key_state_t;

struct plat_key_ops_t
{
    int (*read_raw)(plat_key_state_t *state);
    int (*register_int_cb)(void (*cb)(void));
    int (*enable_int)(void);
    int (*disable_int)(void);
};

struct plat_key_dev_t
{
    plat_dev_t base;
    const plat_key_ops_t *ops;
    void (*cb)(void);
};

void plat_key_dev_register(
    plat_key_dev_t *key, const char *name, const plat_dev_ops_t *base_ops, const plat_key_ops_t *ops, void *priv);
int plat_key_dev_init(plat_key_dev_t *key);
int plat_key_dev_deinit(plat_key_dev_t *key);
int plat_key_dev_cb_register(plat_key_dev_t *key, void (*cb)(void));
int plat_key_dev_read_raw(plat_key_dev_t *key, plat_key_state_t *state);
int plat_key_dev_enable_int(plat_key_dev_t *key);
int plat_key_dev_disable_int(plat_key_dev_t *key);

#endif // PLAT_KEY_H
