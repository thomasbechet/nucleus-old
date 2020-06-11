#ifndef NU_MEMORY_H
#define NU_MEMORY_H

#include <stddef.h>
#include <stdint.h>

#include "platform.h"

typedef void* nu_ptr_t;
typedef void* nu_pfn_t;

NU_API nu_ptr_t nu_malloc(size_t s);
NU_API nu_ptr_t nu_realloc(nu_ptr_t p, size_t s);
NU_API nu_ptr_t nu_calloc(size_t n, size_t s);
NU_API void nu_free(nu_ptr_t p);

NU_API uint64_t nu_memory_total_alloc(void);
NU_API uint64_t nu_memory_total_free(void);

#endif