#include <nucleus/module/ecs/plugin/chunk.h>

#include <nucleus/module/ecs/plugin/archetype.h>

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

nu_result_t nuecs_chunk_allocate(nuecs_archetype_data_t *archetype, nuecs_chunk_data_t **chunk)
{
    /* compute sizes */
    uint32_t total_size = sizeof(nuecs_chunk_data_t);
    const uint32_t indice_table_offset = total_size;
    total_size += (NUECS_CHUNK_SIZE * sizeof(uint32_t) * 2);
    const uint32_t component_list_ptr_offset = total_size;
    total_size += sizeof(nuecs_component_data_ptr_t) * archetype->component_count;
    const uint32_t data_offset = total_size;
    for (uint32_t i = 0; i < archetype->component_count; i++) {
        total_size += archetype->data_sizes[i] * NUECS_CHUNK_SIZE;
    }

    /* allocate memory */
    nuecs_chunk_data_t *chunk_data = (nuecs_chunk_data_t*)nu_malloc(total_size);
    
    /* setup header */
    chunk_data->archetype           = archetype;
    chunk_data->size                = 0;
    chunk_data->free_count          = 0;
    chunk_data->indice_table        = (uint32_t*)((uint8_t*)chunk_data + indice_table_offset);
    chunk_data->component_list_ptrs = (nuecs_component_data_ptr_t*)((uint8_t*)chunk_data + component_list_ptr_offset);

    /* configure component list pointers */
    uint8_t *ptr = (uint8_t*)chunk_data + data_offset;
    for (uint32_t i = 0; i < archetype->component_count; i++) {
        chunk_data->component_list_ptrs[i] = ptr;
        ptr += (archetype->data_sizes[i] * NUECS_CHUNK_SIZE);
    }

    /* return chunk */
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

    return NU_SUCCESS;
}
nu_result_t nuecs_chunk_remove(
    nuecs_chunk_data_t *chunk,
    uint32_t id
)
{
    const uint32_t component_count = chunk->archetype->component_count;
    const uint32_t *sizes          = chunk->archetype->data_sizes;
    uint32_t last_index = chunk->size - 1;
    uint32_t index      = chunk->indice_table[ID_TO_INDEX(id)];
    for (uint32_t i = 0; i < component_count; i++) {
        uint8_t *last_data = (uint8_t*)chunk->component_list_ptrs[i] + sizes[i] * last_index;
        uint8_t *id_data   = (uint8_t*)chunk->component_list_ptrs[i] + sizes[i] * index;
        swap_memory_block(last_data, id_data, sizes[i]);
    }
    chunk->size--;

    uint32_t last_id = chunk->indice_table[last_index];
    chunk->indice_table[ID_TO_INDEX(last_id)]    = index;
    chunk->indice_table[INDEX_TO_ID(index)]      = last_id;
    chunk->indice_table[INDEX_TO_ID(last_index)] = id;
    chunk->free_count++;

    return NU_SUCCESS;
}
nu_result_t nuecs_chunk_transfer(
    nuecs_chunk_data_t *chunk,
    nuecs_chunk_data_t *dst,
    uint32_t id,
    uint32_t *dst_id
)
{
    /* create new entry */
    uint32_t new_id;
    nuecs_chunk_add(dst, &new_id); /* not initialized */

    /* copy data */
    for (uint32_t i = 0; i < dst->archetype->component_count; i++) {
        /* find source component index */
        uint32_t src_index;
        if (nuecs_archetype_find_component_index(chunk->archetype, dst->archetype->component_ids[i], &src_index) != NU_FAILURE) {
            /* find source component data */
            nuecs_component_data_ptr_t ptr;
            nuecs_chunk_get_component(chunk, id, src_index, &ptr);
            /* write component data */
            nuecs_chunk_write_component(dst, new_id, i, ptr);
        }
    }

    /* remove entry */
    nuecs_chunk_remove(chunk, id);

    /* save new id */
    *dst_id = new_id;

    return NU_SUCCESS;
}
nu_result_t nuecs_chunk_get_component(
    nuecs_chunk_data_t *chunk,
    uint32_t id,
    uint32_t component_index,
    nuecs_component_data_ptr_t *component
)
{
    uint32_t index = chunk->indice_table[ID_TO_INDEX(id)];
    *component = (nuecs_component_data_ptr_t)((uint8_t*)chunk->component_list_ptrs[component_index] + chunk->archetype->data_sizes[component_index] * index);

    return NU_SUCCESS;
}
nu_result_t nuecs_chunk_write_component(
    nuecs_chunk_data_t *chunk,
    uint32_t id,
    uint32_t component_index,
    nuecs_component_data_ptr_t data
)
{
    uint32_t index = chunk->indice_table[ID_TO_INDEX(id)];
    const uint32_t size = chunk->archetype->data_sizes[component_index];
    memcpy((uint8_t*)chunk->component_list_ptrs[component_index] + size * index, data, size);

    return NU_SUCCESS;
}