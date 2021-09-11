#include <nucleus/core/utility/container/hashmap.h>

#include <nucleus/core/utility/container/array.h>

typedef struct {
    uint32_t key_size;
    uint32_t value_size;
    uint32_t pair_size;
    nu_array_t data;
} nu_hashmap_header_t;