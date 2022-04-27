#ifndef NU_VECTOR_H
#define NU_VECTOR_H

#include <nucleus/core/utility/platform.h>
#include <nucleus/core/utility/macro.h>

typedef void* nu_vector_t;
typedef int32_t (*nu_vector_cmp_pfn_t)(const void *a, const void *b);
typedef bool (*nu_vector_eq_pfn_t)(const void *a, const void *b);

NU_API void nu_vector_allocate_(nu_vector_t *vec, size_t object_size);
NU_API void nu_vector_free_(nu_vector_t vec);
NU_API void nu_vector_clear(nu_vector_t vec);
NU_API bool nu_vector_is_empty(nu_vector_t vec);
NU_API void *nu_vector_get_last_(nu_vector_t vec);
NU_API bool nu_vector_find_index(nu_vector_t vec, nu_vector_eq_pfn_t eq, const void *user, uint32_t *index);
NU_API uint32_t nu_vector_get_size(nu_vector_t vec);
NU_API uint32_t nu_vector_get_capacity(nu_vector_t vec);
NU_API uint32_t nu_vector_get_allocated_memory(nu_vector_t vec);
NU_API void *nu_vector_push_(nu_vector_t *vec);
NU_API bool nu_vector_pop(nu_vector_t vec);
NU_API void nu_vector_resize(nu_vector_t *vec, uint32_t size);
NU_API void nu_vector_swap(nu_vector_t vec, uint32_t first, uint32_t second);
NU_API void nu_vector_swap_last(nu_vector_t vec, uint32_t i);
NU_API void nu_vector_swap_last_pop(nu_vector_t vec, uint32_t i);
NU_API void *nu_vector_insert_(nu_vector_t *vec, uint32_t i);
NU_API void nu_vector_erase(nu_vector_t vec, uint32_t i);

NU_API bool nu_vector_eq_int32(const void *a, const void *b);

#define nu_vector(type) type*

#define nu_vector_allocate(pvec) nu_vector_allocate_((nu_vector_t*)pvec, sizeof(**pvec))
#define nu_vector_free(vec) nu_vector_free_((nu_vector_t)vec)
#define nu_vector_get_last(vec) ((NU_TYPEOF(vec))nu_vector_get_last_((nu_vector_t)vec))
#define nu_vector_push_value(pvec, value) (*((NU_TYPEOF(*pvec))nu_vector_push_((nu_vector_t*)pvec)) = value)
#define nu_vector_push(pvec) ((NU_TYPEOF(*pvec))nu_vector_push_((nu_vector_t*)pvec))
#define nu_vector_insert(pvec, i, value) (*((NU_TYPEOF(*pvec))nu_vector_insert_((nu_vector_t*)pvec, i)) = value)

#endif