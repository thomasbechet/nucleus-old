#ifndef NU_INTERRUPT_H
#define NU_INTERRUPT_H

#include <nucleus/core/utility/platform.h>

NU_API void nu_interrupt_(const char *file, uint64_t line);

#define nu_interrupt() nu_interrupt_(__FILE__, __LINE__)

#endif