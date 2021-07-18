#ifndef NU_INDEXED_ARRAY_H
#define NU_INDEXED_ARRAY_H

#include <nucleus/core/common/common.h>

NU_DECLARE_HANDLE(nu_indexed_array_t);

NU_API void nu_indexed_array_allocate(uint32_t object_size, nu_indexed_array_t *array);
NU_API void nu_indexed_array_free(nu_indexed_array_t array);
NU_API void nu_indexed_array_add(nu_indexed_array_t array, void *object, uint32_t *id);
NU_API void nu_indexed_array_remove(nu_indexed_array_t array, uint32_t id);
NU_API void *nu_indexed_array_get(nu_indexed_array_t array, uint32_t id); 
NU_API uint32_t nu_indexed_array_get_size(nu_indexed_array_t array);
NU_API void *nu_indexed_array_get_data(nu_indexed_array_t array);
NU_API uint32_t nu_indexed_array_get_allocated_memory(nu_indexed_array_t array);
NU_API uint32_t nu_indexed_array_get_capacity(nu_indexed_array_t array);
NU_API uint32_t nu_indexed_array_get_index_capacity(nu_indexed_array_t array);

#endif