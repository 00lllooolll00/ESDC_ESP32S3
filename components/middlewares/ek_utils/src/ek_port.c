#include "platform.h"
#include "ek_mem.h"

void *_ek_malloc(size_t size)
{
    return malloc(size);
}

void _ek_free(void *ptr)
{
    return free(ptr);
}

void *_ek_realloc(void *ptr, size_t size)
{
    return realloc(ptr, size);
}
