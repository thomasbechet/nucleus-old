#include <nucleus/module/ecs/plugin/chunk.h>

#define ID_TO_INDEX(id)    id    * 2 + 0
#define INDEX_TO_ID(index) index * 2 + 1

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

nu_result_t nuecs_chunk_allocate(nuecs_archetype_t *archetype, nuecs_chunk_data_t **chunk)
{
    uint32_t total_size = sizeof(nuecs_chunk_data_t);
    const uint32_t indice_table_offset = total_size;
    total_size += (NUECS_CHUNK_SIZE * sizeof(uint32_t) * 2);
    const uint32_t data_offset = total_size;
    for (uint32_t i = 0; i < archetype->type_count; i++) {
        total_size += archetype->data_sizes[i] * NUECS_CHUNK_SIZE;
    }

    nuecs_chunk_data_t *chunk_data = (nuecs_chunk_data_t*)nu_malloc(total_size);
    
    chunk_data->archetype  = archetype;
    chunk_data->size       = 0;
    chunk_data->free_count = 0;

    chunk_data->indice_table = (uint32_t*)((uint8_t*)chunk_data + indice_table_offset);
    chunk_data->data         = (uint8_t*)chunk_data + data_offset;

    *chunk = chunk_data;

    return NU_SUCCESS;
}
nu_result_t nuecs_chunk_free(nuecs_chunk_data_t *chunk)
{
    nu_free(chunk);

    return NU_SUCCESS;
}
nu_result_t nuecs_chunk_add(
    nuecs_chunk_data_t *chunk,
    const nuecs_component_data_ptr_t *data,
    uint32_t *id
)
{
    uint32_t size = chunk->size++;
    if (chunk->free_count) {
        uint32_t free_id = chunk->indice_table[INDEX_TO_ID(size)];
        chunk->indice_table[ID_TO_INDEX(free_id)] = size;
        chunk->indice_table[INDEX_TO_ID(size)]    = free_id;
        chunk->free_count--;
        *id = free_id;
    } else {
        chunk->indice_table[ID_TO_INDEX(size)] = size;
        chunk->indice_table[INDEX_TO_ID(size)] = size;
        *id = size;
    }

    const uint32_t type_count = chunk->archetype->type_count;
    const uint32_t *sizes     = chunk->archetype->data_sizes;
    
    uint32_t offset = 0;
    for (uint32_t i = 0; i < type_count; i++) {
        memcpy(chunk->data + offset + sizes[i] * size, data[i], sizes[i]);
        offset += sizes[i] * NUECS_CHUNK_SIZE;
    }

    return NU_SUCCESS;
}
nu_result_t nuecs_chunk_remove(
    nuecs_chunk_data_t *chunk,
    uint32_t id
)
{
    const uint32_t type_count = chunk->archetype->type_count;
    const uint32_t *sizes     = chunk->archetype->data_sizes;
    uint32_t last_index = chunk->size - 1;
    uint32_t index      = chunk->indice_table[ID_TO_INDEX(id)];
    uint32_t offset     = 0;
    for (uint32_t i = 0; i < type_count; i++) {
        uint8_t *last_data = chunk->data + offset + sizes[i] * last_index;
        uint8_t *id_data   = chunk->data + offset + sizes[i] * index;
        swap_memory_block(last_data, id_data, sizes[i]);
        offset += sizes[i] * NUECS_CHUNK_SIZE;
    }
    chunk->size--;

    uint32_t last_id = chunk->indice_table[last_index];
    chunk->indice_table[ID_TO_INDEX(last_id)]    = index;
    chunk->indice_table[INDEX_TO_ID(index)]      = last_id;
    chunk->indice_table[INDEX_TO_ID(last_index)] = id;
    chunk->free_count++;

    return NU_SUCCESS;
}