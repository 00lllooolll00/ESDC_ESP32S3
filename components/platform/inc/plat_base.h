#ifndef PLAT_BASE_H
#define PLAT_BASE_H

#include "common_header.h"

typedef struct plat_dev_t plat_dev_t;
typedef struct plat_dev_ops_t plat_dev_ops_t;

typedef enum
{
    PLAT_STATE_NONE = 0,
    PLAT_STATE_READY,
    PLAT_STATE_BUSY,
    PLAT_STATE_SUSPEND,
    PLAT_STATE_ERROR,

    PLAT_STATE_MAX,
} plat_state_t;

struct plat_dev_ops_t
{
    int (*init)(plat_dev_t *dev);
    int (*deinit)(plat_dev_t *dev);
    int (*suspend)(plat_dev_t *dev);
    int (*resume)(plat_dev_t *dev);
};

struct plat_dev_t
{
    const char *name;
    plat_state_t state;
    const plat_dev_ops_t *base_ops;
    void *priv;
    SemaphoreHandle_t lock;
};

#define PLAT_GET_BASE(ptr) ((plat_dev_t *)(ptr))
#define PLAT_DEV_CHECK(ptr) \
    if (PLAT_GET_BASE(ptr)->state == PLAT_STATE_BUSY || PLAT_GET_BASE(ptr)->state == PLAT_STATE_SUSPEND) return EBUSY

#define PLAT_DEV_LOCK(ptr) \
    (xSemaphoreTake(PLAT_GET_BASE(ptr)->lock, portMAX_DELAY), PLAT_GET_BASE(ptr)->state = PLAT_STATE_BUSY)

#define PLAT_DEV_UNLOCK(ptr) (xSemaphoreGive(PLAT_GET_BASE(ptr)->lock), PLAT_GET_BASE(ptr)->state = PLAT_STATE_READY)

#define PLAT_DEV_LOCK_FROM_ISR(ptr, ptr_higher) \
    ((xSemaphoreTakeFromISR(PLAT_GET_BASE(ptr)->lock, ptr_higher)), (PLAT_GET_BASE(ptr)->state = PLAT_STATE_BUSY))

#define PLAT_DEV_UNLOCK_FROM_ISR(ptr, ptr_higher) \
    ((xSemaphoreGiveFromISR(PLAT_GET_BASE(ptr)->lock, ptr_higher)), (PLAT_GET_BASE(ptr)->state = PLAT_STATE_READY))

void plat_dev_register(plat_dev_t *dev, const char *name, const plat_dev_ops_t *base_ops, void *priv);
int plat_dev_init(plat_dev_t *dev);
int plat_dev_deinit(plat_dev_t *dev);
int plat_dev_suspend(plat_dev_t *dev);
int plat_dev_resume(plat_dev_t *dev);

#endif // PLAT_BASE_H
