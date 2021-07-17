#include <nucleus/core/utils/array.h>

#include <nucleus/core/memory/memory.h>

#define ARRAY_DEFAULT_CAPACITY 10

/*
    +--------+------------+----------+
    | header | swap space | data ... |
    +--------+------------+----------+
*/

typedef struct {
    uint32_t size;
    uint32_t capacity;
    uint32_t object_size;
} nu_array_header_t;

static inline nu_array_header_t *nu_array_get_header_(nu_array_t array)
{
    return (nu_array_header_t*)array;
}
static inline void *nu_array_get_data_(nu_array_t array, uint32_t object_size)
{
    return (char*)array + sizeof(nu_array_header_t) + object_size;
}
static inline void *nu_array_get_swap_space_(nu_array_t array)
{
    return (char*)array + sizeof(nu_array_header_t);
}

void nu_array_allocate(uint32_t object_size, nu_array_t *array)
{
    NU_ASSERT(object_size > 0);

    *array = (nu_array_t)nu_malloc(sizeof(nu_array_header_t) + object_size * (ARRAY_DEFAULT_CAPACITY + 1));
    nu_array_header_t *header = nu_array_get_header_(*array);
    header->capacity    = ARRAY_DEFAULT_CAPACITY;
    header->object_size = object_size;
    header->size        = 0;
}
void nu_array_allocate_capacity(uint32_t object_size, uint32_t capacity, nu_array_t *array)
{
    NU_ASSERT(object_size > 0);
    NU_ASSERT(capacity > 0);

    *array = (nu_array_t)nu_malloc(sizeof(nu_array_header_t) + object_size * (capacity + 1));
    nu_array_header_t *header = nu_array_get_header_(*array);
    header->capacity    = capacity;
    header->object_size = object_size;
    header->size        = 0;
}
void nu_array_allocate_from(const void *data, uint32_t object_count, uint32_t object_size, nu_array_t *array)
{
    NU_ASSERT(object_count > 0 && object_size > 0);

    *array = (nu_array_t)nu_malloc(sizeof(nu_array_header_t) + object_size * (object_count + 1));
    nu_array_header_t *header = nu_array_get_header_(*array);
    header->capacity    = object_count;
    header->object_size = object_size;
    header->size        = object_count;

    /* copy data content to the array */
    memcpy(nu_array_get_data_(*array, object_size), data, object_count * object_size);
}
void nu_array_free(nu_array_t array)
{
    nu_free(array);
}
void *nu_array_get_data(nu_array_t array)
{
    nu_array_header_t *header = nu_array_get_header_(array);
    return nu_array_get_data_(array, header->object_size);
}
const void *nu_array_get_data_const(nu_array_t array)
{
    return nu_array_get_data(array);
}
void nu_array_clear(nu_array_t array)
{
    nu_array_header_t *header = nu_array_get_header_(array);
    header->size = 0;
}
void *nu_array_get(nu_array_t array, uint32_t index)
{
    nu_array_header_t *header = nu_array_get_header_(array);
    NU_ASSERT(index < header->size);
    return (char*)nu_array_get_data_(array, header->object_size) + header->object_size * index;
}
void *nu_array_get_last(nu_array_t array)
{
    nu_array_header_t *header = nu_array_get_header_(array);
    NU_ASSERT(header->size > 0);
    return (char*)nu_array_get_data_(array, header->object_size) + header->object_size * (header->size - 1);
}
uint32_t nu_array_get_size(nu_array_t array)
{
    return nu_array_get_header_(array)->size;
}
uint32_t nu_array_get_capacity(nu_array_t array)
{
    return nu_array_get_header_(array)->capacity;
}
uint32_t nu_array_get_memory_consumption(nu_array_t array)
{
    nu_array_header_t *header = nu_array_get_header_(array);
    return sizeof(nu_array_header_t) + (header->capacity + 1) * header->object_size;
}
void nu_array_push(nu_array_t *array, const void *object)
{
    nu_array_header_t *header = nu_array_get_header_(*array);
    uint32_t size        = header->size;
    uint32_t object_size = header->object_size;

    if (size >= header->capacity) {
        header->capacity *= 2;
        header->size++;
        *array = nu_realloc(*array, sizeof(nu_array_header_t) + object_size * (header->capacity + 1));
    } else {
        header->size++;
    }
    memcpy((char*)nu_array_get_data_(*array, object_size) + object_size * size, object, object_size);
}
bool nu_array_pop(nu_array_t array)
{
    nu_array_header_t *header = nu_array_get_header_(array);
    if (header->size > 0) {
        header->size--;
        return true;
    }
    return false;
}
void nu_array_swap(nu_array_t array, uint32_t first, uint32_t second)
{
    nu_array_header_t *header = nu_array_get_header_(array);
    NU_ASSERT(first < header->size);
    NU_ASSERT(second < header->size);
    if (first != second) {
        uint32_t object_size = header->object_size;
        void *data = nu_array_get_data_(array, object_size);
        void *swap_space = nu_array_get_swap_space_(array);
        /* copy first to the swap space */
        memcpy(swap_space, (char*)data + first * object_size, object_size);
        /* copy second to first */
        memcpy((char*)data + first * object_size, (char*)data + second * object_size, object_size);
        /* copy swap space to second */
        memcpy((char*)data + second * object_size, swap_space, object_size);
    }
}
void nu_array_swap_last(nu_array_t array, uint32_t index)
{
    nu_array_header_t *header = nu_array_get_header_(array);
    nu_array_swap(array, index, header->size - 1);
}