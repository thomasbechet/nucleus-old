#include <nucleus/core/utility/container/indexed_array.h>

#include <nucleus/core/coresystem/memory/memory.h>

#define META_SIZE   2
#define ID_TO_INDEX 0
#define INDEX_TO_ID 1
#define FREE_ID     1

typedef struct {
    uint32_t free_count;
    nu_array_t meta;
    nu_array_t data;
} nu_indexed_array_header_t;

void nu_indexed_array_allocate(nu_indexed_array_t *array, uint32_t object_size)
{
    *array = (nu_indexed_array_t)nu_malloc(sizeof(nu_indexed_array_header_t));
    nu_indexed_array_header_t *header = (nu_indexed_array_header_t*)(*array); 
    header->free_count = 0;
    nu_array_allocate(&header->data, object_size);
    nu_array_allocate(&header->meta, sizeof(uint32_t) * META_SIZE);
}
void nu_indexed_array_free(nu_indexed_array_t array)
{
    nu_indexed_array_header_t *header = (nu_indexed_array_header_t*)array;
    nu_array_free(header->data);
    nu_array_free(header->meta);
    nu_free(array);
}
void nu_indexed_array_add(nu_indexed_array_t array, void *object, uint32_t *id)
{
    nu_indexed_array_header_t *header = (nu_indexed_array_header_t*)array;

    if (header->free_count) {
        uint32_t size = nu_array_get_size(header->data);
        nu_array_push(header->data, object);
        uint32_t *meta;
        nu_array_get_data(header->meta, &meta, NULL);
        uint32_t free_id = meta[META_SIZE * size + FREE_ID];
        meta[META_SIZE * free_id + ID_TO_INDEX] = size;
        meta[META_SIZE * size    + INDEX_TO_ID] = free_id;
        header->free_count--;
        *id = free_id;
    } else {
        uint32_t size = nu_array_get_size(header->data);
        nu_array_push(header->data, object);
        nu_array_push(header->meta, NULL);
        uint32_t *meta;
        nu_array_get_data(header->meta, &meta, NULL);
        meta[META_SIZE * size + ID_TO_INDEX] = size;
        meta[META_SIZE * size + INDEX_TO_ID] = size;
        *id = size;
    }
}
void nu_indexed_array_remove(nu_indexed_array_t array, uint32_t id)
{
    nu_indexed_array_header_t *header = (nu_indexed_array_header_t*)array;

    uint32_t *meta;
    nu_array_get_data(header->meta, &meta, NULL);
    uint32_t last_index = nu_array_get_size(header->data) - 1;
    uint32_t index      = meta[META_SIZE * id + ID_TO_INDEX];
    nu_array_swap_last(header->data, index);
    nu_array_pop(header->data);

    uint32_t last_id = meta[META_SIZE * last_index + INDEX_TO_ID];
    meta[META_SIZE * last_id    + ID_TO_INDEX] = index;
    meta[META_SIZE * index      + INDEX_TO_ID] = last_id;
    meta[META_SIZE * last_index + FREE_ID]     = id;
    header->free_count++;
}
void *nu_indexed_array_get(nu_indexed_array_t array, uint32_t id)
{
    nu_indexed_array_header_t *header = (nu_indexed_array_header_t*)array;
    uint32_t *meta;
    nu_array_get_data(header->meta, &meta, NULL);
    return nu_array_get(header->data, meta[META_SIZE * id + ID_TO_INDEX]);
}
bool nu_indexed_array_find_id(nu_indexed_array_t array, nu_array_find_pfn_t find_pfn, const void *user, uint32_t *id)
{
    nu_indexed_array_header_t *header = (nu_indexed_array_header_t*)array;
    uint32_t *meta;
    nu_array_get_data(header->meta, &meta, NULL);
    uint32_t index;
    if (!nu_array_find_index(header->data, find_pfn, user, &index)) {
        return false;
    }
    *id = meta[META_SIZE * index + INDEX_TO_ID];
    return true;
}
void nu_indexed_array_clear(nu_indexed_array_t array)
{
    nu_indexed_array_header_t *header = (nu_indexed_array_header_t*)array;
    nu_array_clear(header->data);
    nu_array_clear(header->meta);
    header->free_count = 0;
}
uint32_t nu_indexed_array_get_size(nu_indexed_array_t array)
{
    nu_indexed_array_header_t *header = (nu_indexed_array_header_t*)array;
    return nu_array_get_size(header->data);
}
void nu_indexed_array_get_data(nu_indexed_array_t array, void *pdata, uint32_t *size)
{
    nu_indexed_array_header_t *header = (nu_indexed_array_header_t*)array;
    nu_array_get_data(header->data, pdata, size);
}
uint32_t nu_indexed_array_get_allocated_memory(nu_indexed_array_t array)
{
    nu_indexed_array_header_t *header = (nu_indexed_array_header_t*)array;
    return nu_array_get_allocated_memory(header->data) + nu_array_get_allocated_memory(header->meta) + sizeof(nu_indexed_array_header_t);
}
uint32_t nu_indexed_array_get_capacity(nu_indexed_array_t array)
{
    nu_indexed_array_header_t *header = (nu_indexed_array_header_t*)array;
    return nu_array_get_capacity(header->data);
}