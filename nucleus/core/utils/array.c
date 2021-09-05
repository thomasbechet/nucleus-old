#include <nucleus/core/utils/array.h>

#include <nucleus/core/coresystem/memory/memory.h>

#define ARRAY_DEFAULT_CAPACITY 10

/*
    +--------+------------+
    | header | swap space |
    +--------+------------+
        |
        |    +----------+
        ---->| data ... |
             +----------+
*/

typedef struct {
    uint32_t size;
    uint32_t capacity;
    uint32_t object_size;
    char *data;
} nu_array_header_t;

void nu_array_allocate(uint32_t object_size, nu_array_t *array)
{
    NU_ASSERT(object_size > 0);
    *array = (nu_array_t)nu_malloc(sizeof(nu_array_header_t) + object_size);
    nu_array_header_t *header = (nu_array_header_t*)(*array);
    header->size        = 0;
    header->capacity    = ARRAY_DEFAULT_CAPACITY;
    header->object_size = object_size;
    header->data        = nu_malloc(object_size * ARRAY_DEFAULT_CAPACITY);
}
void nu_array_allocate_capacity(uint32_t object_size, uint32_t capacity, nu_array_t *array)
{
    NU_ASSERT(object_size > 0);
    NU_ASSERT(capacity > 0);

    *array = (nu_array_t)nu_malloc(sizeof(nu_array_header_t) + object_size);
    nu_array_header_t *header = (nu_array_header_t*)(*array);
    header->size        = 0;
    header->capacity    = capacity;
    header->object_size = object_size;
    header->data        = nu_malloc(object_size * capacity);
}
void nu_array_allocate_from(const void *data, uint32_t object_count, uint32_t object_size, nu_array_t *array)
{
    NU_ASSERT(object_count > 0 && object_size > 0);

    nu_array_allocate_capacity(object_size, object_count, array);
    nu_array_header_t *header = (nu_array_header_t*)(*array);
    header->size = object_count;

    /* copy data content to the array */
    memcpy(header->data, data, object_count * object_size);
}
void nu_array_free(nu_array_t array)
{
    nu_free(((nu_array_header_t*)array)->data);
    nu_free(array);
}
void *nu_array_get_data(nu_array_t array)
{
    return ((nu_array_header_t*)array)->data;
}
const void *nu_array_get_data_const(nu_array_t array)
{
    return nu_array_get_data(array);
}
void nu_array_clear(nu_array_t array)
{
    ((nu_array_header_t*)array)->size = 0;
}
bool nu_array_is_empty(nu_array_t array)
{
    return ((nu_array_header_t*)array)->size == 0;
}
void *nu_array_get(nu_array_t array, uint32_t index)
{
    nu_array_header_t *header = (nu_array_header_t*)array;
    NU_ASSERT(index < header->size);
    return header->data + header->object_size * index;
}
void *nu_array_get_last(nu_array_t array)
{
    nu_array_header_t *header = (nu_array_header_t*)array;
    NU_ASSERT(header->size > 0);
    return header->data + header->object_size * (header->size - 1);
}
bool nu_array_find_index(nu_array_t array, nu_array_find_pfn_t find_pfn, const void *user, uint32_t *index)
{
    nu_array_header_t *header = (nu_array_header_t*)array;
    for (uint32_t i = 0; i < header->size; i++) {
        if (find_pfn(user, &header->data[i * header->object_size])) {
            *index = i;
            return true;
        }
    }
    return false;
}
uint32_t nu_array_get_size(nu_array_t array)
{
    return ((nu_array_header_t*)array)->size;
}
uint32_t nu_array_get_capacity(nu_array_t array)
{
    return ((nu_array_header_t*)array)->capacity;
}
uint32_t nu_array_get_allocated_memory(nu_array_t array)
{
    nu_array_header_t *header = (nu_array_header_t*)array;
    return sizeof(nu_array_header_t) + header->object_size 
        + header->capacity * header->object_size;
}
void nu_array_push(nu_array_t array, const void *object)
{
    nu_array_header_t *header = (nu_array_header_t*)array;
    header->size++;
    if (header->size > header->capacity) {
        header->capacity *= 3;
        header->data = nu_realloc(header->data, header->object_size * header->capacity);
    }
    if (object) {
        memcpy(header->data + header->object_size * (header->size - 1), object, header->object_size);
    }
}
bool nu_array_pop(nu_array_t array)
{
    nu_array_header_t *header = (nu_array_header_t*)array;
    if (header->size > 0) {
        header->size--;
        return true;
    }
    return false;
}
void nu_array_swap(nu_array_t array, uint32_t first, uint32_t second)
{
    nu_array_header_t *header = (nu_array_header_t*)array;
    NU_ASSERT(first < header->size);
    NU_ASSERT(second < header->size);
    if (first != second) {
        void *dfirst = header->data + first * header->object_size;
        void *dsecond = header->data + second * header->object_size;
        void *dswap = header->data + sizeof(nu_array_header_t);
        /* copy first to the swap space */
        memcpy(dswap, dfirst, header->object_size);
        /* copy second to first */
        memcpy(dfirst, dsecond, header->object_size);
        /* copy swap space to second */
        memcpy(dsecond, dswap, header->object_size);
    }
}
void nu_array_swap_last(nu_array_t array, uint32_t index)
{
    nu_array_header_t *header = (nu_array_header_t*)array;
    nu_array_swap(array, index, header->size - 1);
}