#ifndef NU_ARRAY_H
#define NU_ARRAY_H

#include <nucleus/core/common/common.h>

typedef struct nu_array_t_ nu_array_t;

NU_API nu_array_t *nu_array_new(uint32_t object_size);
NU_API void nu_array_delete(nu_array_t *array);
NU_API void nu_array_clear(nu_array_t *array);
NU_API void *nu_array_get(const nu_array_t *array, uint32_t index);
NU_API void *nu_array_get_last(const nu_array_t *array);
NU_API uint32_t nu_array_get_length(const nu_array_t *array);
NU_API uint32_t nu_array_get_capacity(const nu_array_t *array);
NU_API void nu_array_push(nu_array_t *array, const void *object);
NU_API bool nu_array_pop(nu_array_t *array);
NU_API void nu_array_swap_last(nu_array_t *array, uint32_t index);

#endif