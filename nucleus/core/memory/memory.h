#ifndef NU_MEMORY_H
#define NU_MEMORY_H

#include "../common/common.h"

/* private functions */
nu_result_t nu_memory_initialize(void);
nu_result_t nu_memory_terminate(void);

/* public functions */
NU_API nu_ptr_t nu_malloc(size_t s);
NU_API nu_ptr_t nu_realloc(nu_ptr_t p, size_t s);
NU_API nu_ptr_t nu_calloc(size_t n, size_t s);
NU_API void nu_free(nu_ptr_t p);

NU_API uint64_t nu_memory_total_alloc(void);
NU_API uint64_t nu_memory_total_free(void);

#endif