#ifndef NU_ALLOCATOR_API_H
#define NU_ALLOCATOR_API_H

#include <nucleus/core/api/allocator.h>

NU_API nu_allocator_t nu_allocator_create_freelist(const char *name);
NU_API nu_allocator_t nu_allocator_create_linear(const char *name, size_t max_size);
NU_API nu_allocator_t nu_allocator_create_stack(const char *name);
NU_API void nu_allocator_destroy(nu_allocator_t allocator);
NU_API void nu_allocator_reset(nu_allocator_t allocator);
NU_API void *nu_malloc_(nu_allocator_t allocator, size_t size, const char *file, uint32_t line);
NU_API void *nu_malloc_aligned_(nu_allocator_t allocator, size_t size, size_t alignment, const char *file, uint32_t line);
NU_API void *nu_realloc_(nu_allocator_t allocator, void *p, size_t s, const char *file, uint32_t line);
NU_API void *nu_realloc_aligned_(nu_allocator_t allocator, void *p, size_t alignment, size_t s, const char *file, uint32_t line);
NU_API void *nu_calloc_(nu_allocator_t allocator, size_t n, size_t s, const char *file, uint32_t line);
NU_API void nu_free_(nu_allocator_t allocator, void *p, const char *file, uint32_t line);
NU_API void nu_free_aligned_(nu_allocator_t allocator, void *p, const char *file, uint32_t line);

#define nu_malloc(a, size) \
    nu_malloc_(a, size, __FILE__, __LINE__)
#define nu_malloc_aligned(a, size, alignment) \
    nu_malloc_aligned_(a, size, alignment, __FILE__, __LINE__)
#define nu_realloc(a, p, size) \
    nu_realloc_(a, p, size, __FILE__, __LINE__)
#define nu_realloc_aligned(a, p, size) \
    nu_relloc_aligned_(a, p, size, __FILE__, __LINE__)
#define nu_calloc(a, n, size) \
    nu_calloc_(a, n, size, __FILE__, __LINE__)
#define nu_free(a, p) \
    nu_free_(a, p, __FILE__, __LINE__)
#define nu_free_aligned(a, p) \
    nu_free_aligned_(a, p, __FILE__, __LINE__)

#endif