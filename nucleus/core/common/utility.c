#include "utility.h"

#include <nucleus/core/memory/memory.h>

void nu_snprintf(char *str, size_t n, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vsnprintf(str, n, format, args);
    va_end(args);
}