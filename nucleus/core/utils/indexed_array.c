#include <nucleus/core/utils/indexed_array.h>

#include <nucleus/core/coresystem/memory/memory.h>
#include <nucleus/core/utils/array.h>

#define INDEXED_ARRAY_DEFAULT_CAPACITY 10
#define AVAILABLE_INDEX 0x0

/*
    +--------+
    | header |
    +--------+
        |
        |   +-------------+
        --->| indexes ... |
        |   +-------------+
        |
        |   +--------+------------+----------+
        --->| header | swap space | data ... |
            +--------+------------+----------+
*/

typedef struct {
    uint32_t id_to_index;
    uint32_t index_to_id;
} nu_index_pair_t;

typedef struct {
    uint32_t size;
    uint32_t capacity;
    uint32_t next_free_id;
    nu_array_t data;
    nu_array_t free_ids;
    nu_index_pair_t *indexes;
} nu_indexed_array_header_t;

void nu_indexed_array_allocate(uint32_t object_size, nu_indexed_array_t *array)
{
    *array = (nu_indexed_array_t)nu_malloc(sizeof(nu_indexed_array_header_t));
    nu_indexed_array_header_t *header = (nu_indexed_array_header_t*)(*array); 
    header->size         = 0;
    header->capacity     = INDEXED_ARRAY_DEFAULT_CAPACITY;
    header->next_free_id = 0;
    nu_array_allocate_capacity(object_size, INDEXED_ARRAY_DEFAULT_CAPACITY, &header->data);
    nu_array_allocate(sizeof(uint32_t), &header->free_ids);
    header->indexes      = (nu_index_pair_t*)nu_malloc(sizeof(nu_index_pair_t) * INDEXED_ARRAY_DEFAULT_CAPACITY);
}
void nu_indexed_array_free(nu_indexed_array_t array)
{
    nu_indexed_array_header_t *header = (nu_indexed_array_header_t*)array;
    nu_array_free(header->data);
    nu_array_free(header->free_ids);
    nu_free(header->indexes);
    nu_free(array);
}
void nu_indexed_array_add(nu_indexed_array_t array, void *object, uint32_t *id)
{
    nu_indexed_array_header_t *header = (nu_indexed_array_header_t*)array;
    /* no free index available */
    if (header->next_free_id == header->size) {
        /* check if reallocation is needed */
        if (header->size >= header->capacity) {
            header->capacity *= 2;
            header->indexes = (nu_index_pair_t*)nu_realloc(header->indexes, sizeof(nu_index_pair_t) * header->capacity);
        }
        /* get the data index */
        uint32_t data_index = nu_array_get_size(header->data);
        /* save the data index (+1 so it is different from the available marker) */
        header->indexes[header->size].id_to_index = data_index + 1;
        /* save the id index (size of indexes is always < to the size of data */
        header->indexes[data_index].index_to_id = header->size;
        /* save the index place */
        *id = header->size;
        /* add the object */
        nu_array_push(header->data, object);
        /* increase the list size */
        header->size++;
        /* still no available index */
        header->next_free_id = header->size;
    } else {
        /* save the new id */
        *id = header->next_free_id;
        /* get the data index */
        uint32_t index = nu_array_get_size(header->data);
        /* save the object index (+1 so it is different from the available marker) */
        header->indexes[*id].id_to_index = index + 1;
        /* save the id index (size of indexes is always < to the size of data */
        header->indexes[index].index_to_id = *id;
        /* add the object */
        nu_array_push(header->data, object);
        /* find next free index */
        do {
            header->next_free_id++;
        } while (header->next_free_id < header->size &&
            header->indexes[header->next_free_id].id_to_index != AVAILABLE_INDEX);
    }
}
void nu_indexed_array_remove(nu_indexed_array_t array, uint32_t id)
{
    nu_indexed_array_header_t *header = (nu_indexed_array_header_t*)array;
    NU_ASSERT(id < header->size);
    uint32_t index = header->indexes[id].id_to_index;
    NU_ASSERT(index != AVAILABLE_INDEX);
    index--; /* remove the +1 */
    /* update the next free index */
    if (id < header->next_free_id) {
        header->next_free_id = id;
    }
    /* reduce the indexes list size */
    if (id == header->size - 1) {
        header->size--;
        /* update the next free id if needed */
        if (header->next_free_id > header->size) {
            header->next_free_id = header->size;
        }
    }
    /* index pointing to the last object (only pop) */
    uint32_t size = nu_array_get_size(header->data);
    if (index == (size - 1)) {
        nu_array_pop(header->data);
    } else {
        /* swap elements */
        nu_array_swap_last(header->data, index);
        /* remap the moved data index */
        uint32_t old_id = header->indexes[(size - 1)].index_to_id;
        header->indexes[old_id].id_to_index = index + 1;
        header->indexes[index].index_to_id = old_id;
        /* safely remove the object */
        nu_array_pop(header->data);
    }
    /* mark the index place has available */
    header->indexes[id].id_to_index = AVAILABLE_INDEX;
}
void *nu_indexed_array_get(nu_indexed_array_t array, uint32_t id)
{
    nu_indexed_array_header_t *header = (nu_indexed_array_header_t*)array;
    NU_ASSERT(id < header->size);
    uint32_t index = header->indexes[id].id_to_index;
    NU_ASSERT(index != AVAILABLE_INDEX);
    return nu_array_get(header->data, index - 1);
}
uint32_t nu_indexed_array_get_size(nu_indexed_array_t array)
{
    nu_indexed_array_header_t *header = (nu_indexed_array_header_t*)array;
    return nu_array_get_size(header->data);
}
void *nu_indexed_array_get_data(nu_indexed_array_t array)
{
    nu_indexed_array_header_t *header = (nu_indexed_array_header_t*)array;
    return nu_array_get_data(header->data);
}
uint32_t nu_indexed_array_get_allocated_memory(nu_indexed_array_t array)
{
    nu_indexed_array_header_t *header = (nu_indexed_array_header_t*)array;
    return nu_array_get_allocated_memory(header->data)
        + sizeof(nu_indexed_array_header_t) + sizeof(nu_index_pair_t) * header->capacity;
}
uint32_t nu_indexed_array_get_capacity(nu_indexed_array_t array)
{
    nu_indexed_array_header_t *header = (nu_indexed_array_header_t*)array;
    return nu_array_get_capacity(header->data);
}
uint32_t nu_indexed_array_get_index_capacity(nu_indexed_array_t array)
{
    return ((nu_indexed_array_header_t*)array)->capacity;
}