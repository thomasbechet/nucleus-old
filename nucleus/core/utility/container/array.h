#ifndef NU_ARRAY_H
#define NU_ARRAY_H

#include <nucleus/core/utility/platform.h>
#include <nucleus/core/utility/macro.h>

NU_DECLARE_HANDLE(nu_array_t);
typedef bool (*nu_array_find_pfn_t)(const void *user, const void *object);

NU_API void nu_array_allocate(nu_array_t *array, uint32_t object_size);
NU_API void nu_array_allocate_capacity(nu_array_t *array, uint32_t object_size, uint32_t capacity);
NU_API void nu_array_allocate_from(nu_array_t *array, const void *data, uint32_t object_count, uint32_t object_size);
NU_API void nu_array_free(nu_array_t array);
NU_API void *nu_array_get_data(nu_array_t array);
NU_API const void *nu_array_get_data_const(nu_array_t array);
NU_API void nu_array_clear(nu_array_t array);
NU_API bool nu_array_is_empty(nu_array_t array);
NU_API void *nu_array_get(nu_array_t array, uint32_t index);
NU_API void *nu_array_get_last(nu_array_t array);
NU_API bool nu_array_find_index(nu_array_t array, nu_array_find_pfn_t find_pfn, const void *user, uint32_t *index);
NU_API uint32_t nu_array_get_size(nu_array_t array);
NU_API uint32_t nu_array_get_capacity(nu_array_t array);
NU_API uint32_t nu_array_get_allocated_memory(nu_array_t array);
NU_API void nu_array_push(nu_array_t array, const void *object);
NU_API bool nu_array_pop(nu_array_t array);
NU_API void nu_array_swap(nu_array_t array, uint32_t first, uint32_t second);
NU_API void nu_array_swap_last(nu_array_t array, uint32_t index);

#endif