#include "plat_tts.h"

void plat_tts_dev_register(
    plat_tts_dev_t *tts, const char *name, const plat_dev_ops_t *base_ops, const plat_tts_ops_t *ops, void *priv)
{
    plat_dev_register(PLAT_GET_BASE(tts), name, base_ops, priv);
    tts->ops = ops;
}

int plat_tts_dev_init(plat_tts_dev_t *tts)
{
    assert(tts);
    return plat_dev_init(PLAT_GET_BASE(tts));
}

int plat_tts_dev_deinit(plat_tts_dev_t *tts)
{
    assert(tts);
    return plat_dev_deinit(PLAT_GET_BASE(tts));
}

int plat_tts_dev_speak(plat_tts_dev_t *tts, const char *text)
{
    PLAT_DEV_CHECK(tts);
    return tts->ops->speak(text);
}

int plat_tts_dev_enable_amp(plat_tts_dev_t *tts, int enable)
{
    PLAT_DEV_CHECK(tts);
    return tts->ops->enable_amp(enable);
}
