#include "utility.h"

#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "logger.h"
#include "result.h"
#include "memory.h"

char *nu_sprintf(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    size_t size = vsnprintf(NULL, 0, format, args);
    char *buf = nu_malloc(size + 1);
    vsnprintf(buf, size + 1, format, args);
    va_end(args);
    return buf;
}
void nu_interrupt(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    nu_log(NU_FATAL, format, args);
    va_end(args);
    exit(NU_FAILURE);
}