#ifndef NU_API_ALLOCATOR_H
#define NU_API_ALLOCATOR_H

#include <nucleus/core/utility/platform.h>
#include <nucleus/core/utility/macro.h>
#include <nucleus/core/utility/result.h>

NU_DECLARE_HANDLE(nu_allocator_t);

typedef struct {
    nu_allocator_t (*create_freelist)(const char *name);
    nu_allocator_t (*create_linear)(const char *name, size_t max_size);
    nu_allocator_t (*create_stack)(const char *name);
    nu_result_t (*destroy)(nu_allocator_t a);
    void (*reset)(nu_allocator_t allocator);
    void *(*malloc)(nu_allocator_t allocator, size_t size, const char *file, uint32_t line);
    void *(*malloc_aligned)(nu_allocator_t allocator, size_t size, size_t alignment, const char *file, uint32_t line);
    void *(*realloc)(nu_allocator_t allocator, void *p, size_t s, const char *file, uint32_t line);
    void *(*realloc_aligned)(nu_allocator_t allocator, void *p, size_t alignment, size_t s, const char *file, uint32_t line);
    void *(*calloc)(nu_allocator_t allocator, size_t n, size_t s, const char *file, uint32_t line);
    void (*free)(nu_allocator_t allocator, void *p, const char *file, uint32_t line);
    void (*free_aligned)(nu_allocator_t allocator, void *p, const char *file, uint32_t line);
} nu_allocator_api_t;

#endif