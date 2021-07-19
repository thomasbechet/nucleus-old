#include <nucleus/core/utils/indexed_array.h>

#include <nucleus/core/coresystem/memory/memory.h>

#define INDEXED_ARRAY_DEFAULT_CAPACITY 10
#define FREE_INDEX_FLAG 0x0

/*
    +--------+
    | header |
    +--------+
        |
        |   +-------------+
        --->| indexes ... |
        |   +-------------+
        |
        |   +--------+------------+
        --->| header | swap space |
        |   +--------+------------+
        |       |
        |       |    +-------------+
        |       ---->| free_ids... |
        |            +-------------+
        |
        |   +--------+------------+
        --->| header | swap space |
            +--------+------------+
                |
                |    +----------+
                ---->| data ... |
                     +----------+
*/

typedef struct {
    uint32_t id_to_index;
    uint32_t index_to_id;
} nu_index_pair_t;

typedef struct {
    uint32_t size;
    uint32_t capacity;
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
    if (nu_array_get_size(header->free_ids) == 0) {
        /* check if reallocation is needed */
        if (header->size >= header->capacity) {
            header->capacity *= 2;
            header->indexes = (nu_index_pair_t*)nu_realloc(header->indexes, sizeof(nu_index_pair_t) * header->capacity);
        }
        /* save the id and increase the list size */
        *id = header->size++;
    } else {
        /* save the new id */
        *id = *(uint32_t*)nu_array_get_last(header->free_ids);
        /* this id is no longer free */
        nu_array_pop(header->free_ids);
    }
    /* get the data index */
    uint32_t index = nu_array_get_size(header->data);
    /* save the data index (+1 so it is different from the free index flag) */
    header->indexes[*id].id_to_index = index + 1;
    /* save the id index (size of indexes is always < to the size of data */
    header->indexes[index].index_to_id = *id;
    /* add the object */
    nu_array_push(header->data, object);
}
void nu_indexed_array_remove(nu_indexed_array_t array, uint32_t id)
{
    nu_indexed_array_header_t *header = (nu_indexed_array_header_t*)array;
    NU_ASSERT(id < header->size);
    uint32_t index = header->indexes[id].id_to_index;
    NU_ASSERT(index != FREE_INDEX_FLAG);
    index--; /* remove the +1 */
    /* add this id to the free ids */
    nu_array_push(header->free_ids, &id);
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
    /* mark the index place as free */
    header->indexes[id].id_to_index = FREE_INDEX_FLAG;
}
void *nu_indexed_array_get(nu_indexed_array_t array, uint32_t id)
{
    nu_indexed_array_header_t *header = (nu_indexed_array_header_t*)array;
    NU_ASSERT(id < header->size);
    uint32_t index = header->indexes[id].id_to_index;
    NU_ASSERT(index != FREE_INDEX_FLAG);
    return nu_array_get(header->data, index - 1);
}
bool nu_indexed_array_find_id(nu_indexed_array_t array, nu_array_find_pfn_t find_pfn, const void *user, uint32_t *id)
{
    nu_indexed_array_header_t *header = (nu_indexed_array_header_t*)array;
    uint32_t index;
    if (!nu_array_find_index(header->data, find_pfn, user, &index)) {
        return false;
    }
    *id = header->indexes[index].index_to_id;
    NU_ASSERT(*id != FREE_INDEX_FLAG);
    return true;
}
void nu_indexed_array_clear(nu_indexed_array_t array)
{
    nu_indexed_array_header_t *header = (nu_indexed_array_header_t*)array;
    header->size = 0;
    nu_array_clear(header->data);
    nu_array_clear(header->free_ids);
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
    return nu_array_get_allocated_memory(header->data) + nu_array_get_allocated_memory(header->free_ids)
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