#include "vars.h"
#include "common_header.h"

#define VOLUME_MAX 100
#define VOLUME_MIN 0

int32_t g_volume;

int32_t get_var_g_volume()
{
    return g_volume;
}

void set_var_g_volume(int32_t value)
{
    int32_t temp = value;
    temp = EK_CLAMP(temp, VOLUME_MIN, VOLUME_MAX);
    g_volume = temp;
}
