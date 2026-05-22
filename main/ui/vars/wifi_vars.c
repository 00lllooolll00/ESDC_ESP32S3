#include "vars.h"

bool g_wifi_is_connected;

bool get_var_g_wifi_is_connected()
{
    return g_wifi_is_connected;
}

void set_var_g_wifi_is_connected(bool value)
{
    g_wifi_is_connected = value;
}
