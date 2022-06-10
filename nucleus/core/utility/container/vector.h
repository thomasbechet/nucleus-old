#ifndef NU_VECTOR_H
#define NU_VECTOR_H

#include <nucleus/core/api/allocator.h>
#include <nucleus/core/utility/macro.h>
#include <nucleus/core/utility/platform.h>

typedef void* nu_vector_t;
typedef int32_t (*nu_vector_cmp_pfn_t)(const void *a, const void *b);
typedef bool (*nu_vector_eq_pfn_t)(const void *a, const void *b);

NU_API nu_vector_t nu_vector_allocate_capacity_(nu_allocator_t allocator, size_t osize, uint32_t capacity, const char *file, uint32_t line);
NU_API void nu_vector_free_(nu_vector_t vec, const char *file, uint32_t line);
NU_API void nu_vector_clear(nu_vector_t vec);
NU_API bool nu_vector_empty(const nu_vector_t vec);
NU_API void *nu_vector_last_(const nu_vector_t vec, size_t size);
NU_API bool nu_vector_find_index_(const nu_vector_t vec, nu_vector_eq_pfn_t eq, const void *user, uint32_t *index, size_t size);
NU_API void *nu_vector_find_(const nu_vector_t vec, nu_vector_eq_pfn_t eq, const void *user, size_t size);
NU_API uint32_t nu_vector_size(const nu_vector_t vec);
NU_API uint32_t nu_vector_capacity(const nu_vector_t vec);
NU_API uint32_t nu_vector_allocated_memory_(const nu_vector_t vec, size_t size);
NU_API void *nu_vector_push_(nu_vector_t *vec, size_t size, const char *file, uint32_t line);
NU_API bool nu_vector_pop(nu_vector_t vec);
NU_API void nu_vector_resize_(nu_vector_t *vec, uint32_t newsize, size_t size, const char *file, uint32_t line);
NU_API void nu_vector_swap_(nu_vector_t vec, uint32_t first, uint32_t second, size_t size);
NU_API void nu_vector_swap_last_(nu_vector_t vec, uint32_t i, size_t size);
NU_API void nu_vector_swap_last_pop_(nu_vector_t vec, uint32_t i, size_t size);
NU_API void *nu_vector_insert_(nu_vector_t *vec, uint32_t i, size_t size, const char *file, uint32_t line);
NU_API void nu_vector_erase_(nu_vector_t vec, uint32_t i, size_t size);

NU_API bool nu_vector_eq_int32(const void *a, const void *b);

#define nu_vector(type) type*

#define nu_vector_allocate(allocator, type) ((nu_vector(type))nu_vector_allocate_capacity_(allocator, sizeof(type), 0, __FILE__, __LINE__))
#define nu_vector_allocate_capacity(allocator, type, capacity) ((nu_vector(type))nu_vector_allocate_capacity_(allocator, sizeof(type), capacity, __FILE__, __LINE__))
#define nu_vector_free(vec) (nu_vector_free_((nu_vector_t)vec, __FILE__, __LINE__))
#define nu_vector_last(vec) (nu_vector_last_((nu_vector_t)vec), sizeof(*vec))
#define nu_vector_find_index(vec, eq_pfn, user, pindex) (nu_vector_find_index_((nu_vector_t)vec, eq_pfn, user, pindex, sizeof(*vec)))
#define nu_vector_find(vec, eq_pfn, user) (nu_vector_find_((nu_vector_t)vec, eq_pfn, user, sizeof(*vec)))
#define nu_vector_push(pvec) (nu_vector_push_((nu_vector_t*)pvec, sizeof(**pvec), __FILE__, __LINE__))
#define nu_vector_push_value(pvec, value) \
    do { \
        void *p = nu_vector_push_((nu_vector_t*)pvec, sizeof(**pvec), __FILE__, __LINE__); \
        (*pvec)[((size_t)p - (size_t)(*pvec)) / sizeof(**pvec)] = value; \
    } while(0)
#define nu_vector_swap(vec, first, second) (nu_vector_swap_((nu_vector_t)vec, first, second, sizeof(*vec)))
#define nu_vector_swap_last(vec, i) (nu_vector_swap_last_((nu_vector_t)vec, i, sizeof(*vec)))
#define nu_vector_swap_last_pop(vec, i) (nu_vector_swap_last_pop_((nu_vector_t)vec, i, sizeof(*vec)))
#define nu_vector_erase(vec, i) (nu_vector_erase_((nu_vector_t)vec, i, sizeof(*vec)))
#define nu_vector_end(vec) (vec + nu_vector_size(vec))
#define nu_vector_resize(pvec, newsize) (nu_vector_resize_((nu_vector_t*)pvec, newsize, sizeof(**pvec), __FILE__, __LINE__))

#define nu_vector_allocate_s(allocator, osize) (nu_vector_allocate_capacity_(allocator, osize, 0, __FILE__, __LINE__))
#define nu_vector_get_last_s(vec, osize) (nu_vector_get_last_(vec, osize))
#define nu_vector_find_index_s(vec, osize, eq_pfn, user, pindex) (nu_vector_find_index_(vec, eq_pfn, user, pindex, osize))
#define nu_vector_find_s(vec, osize, eq_pfn, user) (nu_vector_find_(vec, eq_pfn, user, osize))
#define nu_vector_push_s(pvec, osize) (nu_vector_push_(pvec, osize, __FILE__, __LINE__))
#define nu_vector_swap_s(vec, osize, first, second) (nu_vector_swap_(vec, first, second, osize))
#define nu_vector_swap_last_s(vec, osize, i) (nu_vector_swap_last_(vec, i, osize))
#define nu_vector_swap_last_pop_s(vec, osize, i) (nu_vector_swap_last_pop_(vec, i, osize))
#define nu_vector_erase_s(vec, osize, i) (nu_vector_erase_(vec, i, osize))
#define nu_vector_get_s(vec, osize, i) (void*)((uint8_t*)vec + osize * i)

#endif