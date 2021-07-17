#ifndef NU_HASHMAP_H
#define NU_HASHMAP_H

#include <nucleus/core/common/common.h>

NU_DECLARE_HANDLE(nu_hashmap_t);

NU_API void nu_hashmap_allocate(uint32_t key_size, uint32_t value_size, nu_hashmap_t *hashmap);
NU_API void nu_hashmap_free(nu_hashmap_t hashmap);
NU_API void nu_hashmap_put(nu_hashmap_t *hashmap, const void *key, const void *value);
NU_API void *nu_hashmap_get(nu_hashmap_t hashmap, const void *key);
NU_API void nu_hashmap_remove(nu_hashmap_t *hashmap, const void *key);

#endif