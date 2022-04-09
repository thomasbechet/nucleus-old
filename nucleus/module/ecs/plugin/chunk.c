#include <nucleus/module/ecs/plugin/chunk.h>

#include <nucleus/module/ecs/plugin/archetype.h>
#include <nucleus/module/ecs/plugin/utility.h>
#include <nucleus/module/ecs/plugin/entity_reference.h>

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

nu_result_t nuecs_chunk_allocate(
    nuecs_archetype_data_t *archetype,
    nuecs_chunk_data_t **chunk
)
{
    /* compute sizes */
    uint32_t total_size = sizeof(nuecs_chunk_data_t);
    const uint32_t rows_offset = total_size;
    total_size += (NUECS_CHUNK_SIZE * sizeof(nuecs_chunk_row_t));
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
    chunk_data->next_version        = 0;
    chunk_data->rows                = (nuecs_chunk_row_t*)((uint8_t*)chunk_data + rows_offset);
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
    nuecs_entity_t *handle
)
{
    NU_ASSERT(chunk->size < NUECS_CHUNK_SIZE);

    /* find next id */
    uint8_t id;
    uint8_t size = chunk->size++;
    if (chunk->free_count) {
        uint8_t free_id = chunk->rows[size].index_to_id;
        chunk->rows[free_id].id_to_index = size;
        chunk->rows[size].index_to_id    = free_id;
        chunk->rows[free_id].version     = chunk->next_version;
        chunk->free_count--;
        id = free_id;
    } else {
        chunk->rows[size].id_to_index = size;
        chunk->rows[size].index_to_id = size;
        chunk->rows[size].version     = chunk->next_version;
        id = size;
    }

    /* update entity row */
    chunk->rows[id].version         = nuecs_next_version(&chunk->next_version);
    chunk->rows[id].reference_index = NUECS_ENTITY_REFERENCE_NONE; 

    /* return handle */
    *handle = NUECS_ENTITY_BUILD_HANDLE(chunk->next_version, id, chunk->id);

    return NU_SUCCESS;
}
nu_result_t nuecs_chunk_remove(
    nuecs_chunk_data_t *chunk,
    nuecs_entity_t handle
)
{
    /* unwrap handle */
    uint8_t version  = NUECS_ENTITY_VERSION(handle);
    uint8_t id       = NUECS_ENTITY_ID(handle);
    uint8_t chunk_id = NUECS_ENTITY_CHUNK(handle);
    NU_ASSERT(chunk->id == chunk_id && chunk->rows[id].version == version);

    /* swap components */
    const uint32_t component_count = chunk->archetype->component_count;
    const uint32_t *sizes          = chunk->archetype->data_sizes;
    uint8_t last_index             = chunk->size - 1;
    uint8_t index                  = chunk->rows[id].id_to_index;
    for (uint32_t i = 0; i < component_count; i++) {
        uint8_t *last_data = (uint8_t*)chunk->component_list_ptrs[i] + sizes[i] * last_index;
        uint8_t *id_data   = (uint8_t*)chunk->component_list_ptrs[i] + sizes[i] * index;
        swap_memory_block(last_data, id_data, sizes[i]);
    }
    chunk->size--;
    
    /* update rows */
    uint8_t last_id = chunk->rows[last_index].id_to_index;
    chunk->rows[last_id].id_to_index    = index;
    chunk->rows[index].index_to_id      = last_id;
    chunk->rows[last_index].index_to_id = id;
    chunk->free_count++;

    return NU_SUCCESS;
}
nu_result_t nuecs_chunk_transfer(
    nuecs_chunk_data_t *src,
    nuecs_chunk_data_t *dst,
    nuecs_entity_t *handle
)
{
    /* create new entry */
    nuecs_entity_t new_handle;
    nuecs_chunk_add(dst, &new_handle); /* not initialized */

    /* copy data */
    for (uint32_t i = 0; i < dst->archetype->component_count; i++) {

        /* find source component index */
        uint32_t src_index;
        if (nuecs_archetype_find_component_index(src->archetype, dst->archetype->component_ids[i], &src_index) != NU_FAILURE) {

            /* find source component data */
            nuecs_component_data_ptr_t ptr;
            nuecs_chunk_get_component(src, *handle, src_index, &ptr);

            /* write component data */
            nuecs_chunk_write_component(dst, new_handle, i, ptr);
        }
    }

    /* remove old entry */
    nuecs_chunk_remove(src, *handle);

    /* save new handle */
    *handle = new_handle;

    return NU_SUCCESS;
}
nu_result_t nuecs_chunk_get_component(
    nuecs_chunk_data_t *chunk,
    nuecs_entity_t handle,
    uint32_t component_index,
    nuecs_component_data_ptr_t *component
)
{
    /* unwrap handle */
    uint8_t version  = NUECS_ENTITY_VERSION(handle);
    uint8_t id       = NUECS_ENTITY_ID(handle);
    uint8_t chunk_id = NUECS_ENTITY_CHUNK(handle);
    NU_ASSERT(chunk->id == chunk_id && chunk->rows[id].version == version);

    /* get component */
    uint32_t index = chunk->rows[id].id_to_index;
    *component = (nuecs_component_data_ptr_t)((uint8_t*)chunk->component_list_ptrs[component_index] + chunk->archetype->data_sizes[component_index] * index);
    
    return NU_SUCCESS;
}
nu_result_t nuecs_chunk_write_component(
    nuecs_chunk_data_t *chunk,
    nuecs_entity_t handle,
    uint32_t component_index,
    nuecs_component_data_ptr_t data
)
{
    /* unwrap handle */
    uint8_t version  = NUECS_ENTITY_VERSION(handle);
    uint8_t id       = NUECS_ENTITY_ID(handle);
    uint8_t chunk_id = NUECS_ENTITY_CHUNK(handle);
    NU_ASSERT(chunk->id == chunk_id && chunk->rows[id].version == version);

    /* write component */
    uint32_t index = chunk->rows[id].id_to_index;
    const uint32_t size = chunk->archetype->data_sizes[component_index];
    memcpy((uint8_t*)chunk->component_list_ptrs[component_index] + size * index, data, size);

    return NU_SUCCESS;
}
bool nuecs_chunk_check_entity(
    nuecs_chunk_data_t *chunk,
    nuecs_entity_t handle
)
{
    /* unwrap handle */
    uint8_t version  = NUECS_ENTITY_VERSION(handle);
    uint8_t id       = NUECS_ENTITY_ID(handle);
    uint8_t chunk_id = NUECS_ENTITY_CHUNK(handle);

    /* check */
    return (chunk->id == chunk_id && chunk->rows[id].version == version);
}
nu_result_t nuecs_chunk_get_entity(
    nuecs_chunk_data_t *chunk,
    uint8_t index,
    nuecs_entity_t *handle
)
{
    NU_ASSERT(index < chunk->size);
    *handle = NUECS_ENTITY_BUILD_HANDLE(chunk->rows[index].version, 
        chunk->rows[index].index_to_id, chunk->id);
    return NU_SUCCESS;
}
nu_result_t nuecs_chunk_set_entity_reference(
    nuecs_chunk_data_t *chunk,
    nuecs_entity_t handle,
    uint32_t index
)
{
    uint8_t id = NUECS_ENTITY_ID(handle);
    chunk->rows[id].reference_index = index;
    return NU_SUCCESS;
}
nu_result_t nuecs_chunk_get_entity_reference(
    nuecs_chunk_data_t *chunk,
    nuecs_entity_t handle,
    uint32_t *index
)
{
    uint8_t id = NUECS_ENTITY_ID(handle);
    *index = chunk->rows[id].reference_index;
    return NU_SUCCESS;
}