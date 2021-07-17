#include <nucleus/core/utils/indexed_array.h>

#include <nucleus/core/memory/memory.h>
#include <nucleus/core/utils/array.h>

#define INDEXED_ARRAY_DEFAULT_CAPACITY 10
#define AVAILABLE_INDEX 0x0

/*
    +--------+-------------+
    | header | indexes ... |
    +--------+-------------+
        |
        |   +--------+------------+----------+
        --->| header | swap space | data ... |
            +--------+------------+----------+
*/

typedef struct {
    uint32_t size;
    uint32_t capacity;
    uint32_t next_free_id;
    nu_array_t data;
} nu_indexed_array_header_t;

static inline nu_indexed_array_header_t *nu_indexed_array_get_header_(nu_indexed_array_t array)
{
    return (nu_indexed_array_header_t*)array;
}
static inline uint32_t *nu_indexed_array_get_indexes_(nu_indexed_array_t array)
{
    return (void*)array + sizeof(nu_indexed_array_header_t);
}

void nu_indexed_array_allocate(uint32_t object_size, nu_indexed_array_t *array)
{
    *array = (nu_indexed_array_t)nu_malloc(sizeof(nu_indexed_array_header_t)
        + sizeof(uint32_t) * INDEXED_ARRAY_DEFAULT_CAPACITY);
    nu_indexed_array_header_t *header = nu_indexed_array_get_header_(*array);
    header->capacity        = INDEXED_ARRAY_DEFAULT_CAPACITY;
    header->size            = 0;
    header->next_free_id    = 0;
    nu_array_allocate_capacity(object_size, INDEXED_ARRAY_DEFAULT_CAPACITY, &header->data);
}
void nu_indexed_array_free(nu_indexed_array_t array)
{
    nu_indexed_array_header_t *header = nu_indexed_array_get_header_(array);
    nu_array_free(header->data);
    nu_free(array);
}
void nu_indexed_array_add(nu_indexed_array_t *array, void *object, uint32_t *id)
{
    nu_indexed_array_header_t *header = nu_indexed_array_get_header_(*array);
    /* no free index available */
    if (header->next_free_id == header->size) {
        /* check if reallocation is needed */
        if (header->size >= header->capacity) {
            header->capacity *= 2;
            *array = nu_realloc(*array, sizeof(nu_indexed_array_t) * header->capacity);
            header = nu_indexed_array_get_header_(*array);
        }
        uint32_t *indexes = nu_indexed_array_get_indexes_(*array);
        /* save the object index (+1 so it is different from the available marker) */
        indexes[header->size] = nu_array_get_size(header->data) + 1;
        /* save the index place */
        *id = header->size;
        /* add the object */
        nu_array_push(&header->data, object);
        /* increase the list size */
        header->size++;
        /* still no available size */
        header->next_free_id = header->size;
    } else {
        /* save the new id */
        *id = header->next_free_id;
        /* save the object index (+1 so it is different from the available marker) */
        uint32_t *indexes = nu_indexed_array_get_indexes_(*array);
        indexes[header->next_free_id] = nu_array_get_size(header->data) + 1;
        /* add the object */
        nu_array_push(&header->data, object);
        /* find next free index */
        do {
            header->next_free_id++;
        } while (header->next_free_id < header->size &&
            indexes[header->next_free_id] != AVAILABLE_INDEX);
    }
}
void nu_indexed_array_remove(nu_indexed_array_t array, uint32_t id)
{
    nu_indexed_array_header_t *header = nu_indexed_array_get_header_(array);
    NU_ASSERT(id < header->size);
    uint32_t *indexes = nu_indexed_array_get_indexes_(array); 
    uint32_t index = indexes[id];
    NU_ASSERT(index != AVAILABLE_INDEX);
    /* update the next free index */
    if (id < header->next_free_id) {
        header->next_free_id = id;
    }
    /* reduce the indexes list size */
    if (id == header->size - 1) {
        header->size--;
        if (header->next_free_id > header->size) {
            header->next_free_id = header->size;
        }
    }
    /* index pointing to the last object (only pop) */
    if (index == nu_array_get_size(header->data)) {
        nu_array_pop(header->data);
    } else {
        /* swap elements (index remapping required) */
        nu_array_swap_last(header->data, index - 1);
        /* find the the place mapping to the moved object */
        uint32_t i;
        for (i = 0; i < header->size; i++) {
            if (indexes[i] == nu_array_get_size(header->data)) {
                indexes[i] = index;
                break;
            }
        }
        NU_ASSERT(i != header->size);
        /* safely remove the object */
        nu_array_pop(header->data);
    }
    /* mark the index place has available */
    indexes[id] = AVAILABLE_INDEX;
}
void *nu_indexed_array_get(nu_indexed_array_t array, uint32_t id)
{
    nu_indexed_array_header_t *header = nu_indexed_array_get_header_(array);
    NU_ASSERT(id < header->size);
    uint32_t index = nu_indexed_array_get_indexes_(array)[id];
    NU_ASSERT(index != AVAILABLE_INDEX);
    return nu_array_get(header->data, index - 1);
}
uint32_t nu_indexed_array_get_size(nu_indexed_array_t array)
{
    nu_indexed_array_header_t *header = nu_indexed_array_get_header_(array);
    return nu_array_get_size(header->data);
}
void *nu_indexed_array_get_data(nu_indexed_array_t array)
{
    nu_indexed_array_header_t *header = nu_indexed_array_get_header_(array);
    return nu_array_get_data(header->data);
}
uint32_t nu_indexed_array_get_memory_consumption(nu_indexed_array_t array)
{
    nu_indexed_array_header_t *header = nu_indexed_array_get_header_(array);
    return nu_array_get_memory_consumption(header->data)
        + sizeof(nu_indexed_array_header_t) + sizeof(uint32_t) * header->capacity;
}