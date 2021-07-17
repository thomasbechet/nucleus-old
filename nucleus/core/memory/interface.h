#ifndef NU_MEMORY_INTERFACE_H
#define NU_MEMORY_INTERFACE_H

#include <nucleus/core/common/common.h>

NU_API void *nu_malloc(size_t s);
NU_API void *nu_realloc(void *p, size_t s);
NU_API void *nu_calloc(size_t n, size_t s);
NU_API void nu_free(void *p);

NU_API uint64_t nu_memory_total_alloc(void);
NU_API uint64_t nu_memory_total_free(void);
NU_API uint64_t nu_memory_total_realloc(void);

#endif