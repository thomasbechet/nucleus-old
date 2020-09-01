#ifndef NU_UTILITY_H
#define NU_UTILITY_H

#include "platform.h"

typedef uint32_t nu_id_t;

NU_API char *nu_sprintf(const char *format, ...);
NU_API void nu_interrupt(const char *format, ...);

#endif