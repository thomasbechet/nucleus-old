#include <nucleus/module/ecs/plugin/chunk.h>

static inline void swap_memory_block(uint8_t *a, uint8_t *b, uint32_t size)
{
    const uint8_t *end = a + size;
    uint8_t *pa = a;
    uint8_t *pb = b;
    for (; pa < end; pa++, pb++) {
        uint8_t temp = *pa;
        *pa = *pb;
        *pb = temp;
    }
}

nu_result_t nuecs_chunk_create(nuecs_chunk_data_t *chunk, uint32_t total_data_size, uint32_t entity_count)
{
    chunk->data           = (uint8_t*)nu_malloc(total_data_size);
    chunk->indices_to_ids = (uint32_t*)nu_malloc(sizeof(uint32_t) * entity_count);
    chunk->ids_to_indices = (uint32_t*)nu_malloc(sizeof(uint32_t) * entity_count);
    chunk->free_count     = 0;
    chunk->size           = 0;

    return NU_SUCCESS;
}
nu_result_t nuecs_chunk_destroy(nuecs_chunk_data_t *chunk)
{
    nu_free(chunk->data);
    nu_free(chunk->indices_to_ids);
    nu_free(chunk->ids_to_indices);

    return NU_SUCCESS;
}
nu_result_t nuecs_chunk_add(
    nuecs_chunk_data_t *chunk, 
    const uint32_t *offsets, 
    const uint32_t *sizes,
    nuecs_component_data_ptr_t *data,
    uint32_t type_count,
    uint32_t *id
)
{
    uint32_t size = chunk->size++;
    if (chunk->free_count) {
        uint32_t free_id = chunk->indices_to_ids[size];
        chunk->ids_to_indices[free_id] = size;
        chunk->indices_to_ids[size]    = free_id;
        chunk->free_count--;
        *id = free_id;
    } else {
        chunk->ids_to_indices[size] = size;
        chunk->indices_to_ids[size] = size;
        *id = size;
    }

    for (uint32_t i = 0; i < type_count; i++) {
        memcpy(chunk->data + offsets[i] + sizes[i] * size, data[i], sizes[i]);
    }

    return NU_SUCCESS;
}
nu_result_t nuecs_chunk_remove(
    nuecs_chunk_data_t *chunk,
    const uint32_t *offsets,
    const uint32_t *sizes,
    uint32_t type_count,
    uint32_t id
)
{
    uint32_t last_index = chunk->size - 1;
    uint32_t index      = chunk->ids_to_indices[id];
    for (uint32_t i = 0; i < type_count; i++) {
        uint8_t *last_data = chunk->data + offsets[i] + sizes[i] * last_index;
        uint8_t *id_data   = chunk->data + offsets[i] + sizes[i] * index;
        swap_memory_block(last_data, id_data, sizes[i]);
    }
    chunk->size--;

    uint32_t last_id = chunk->indices_to_ids[last_index];
    chunk->ids_to_indices[last_id]    = index;
    chunk->indices_to_ids[index]      = last_id;
    chunk->indices_to_ids[last_index] = id;
    chunk->free_count++;

    return NU_SUCCESS;
}