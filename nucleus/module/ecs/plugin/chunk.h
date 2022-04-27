#ifndef NUECS_CHUNK_H
#define NUECS_CHUNK_H

#include <nucleus/module/ecs/plugin/archetype.h>
#include <nucleus/module/ecs/plugin/types_public.h>

/* |--VERSION--|---ID---|---CHUNK---| */
/* |   8bits   |  8bits |  16bits   | */

#define NUECS_CHUNK_SIZE 256

#define NUECS_ENTITY_VERSION_MASK 0xFF000000
#define NUECS_ENTITY_ID_MASK      0x00FF0000
#define NUECS_ENTITY_CHUNK_MASK   0x0000FFFF

#define NUECS_ENTITY_VERSION(handle) ({ \
    uint32_t id; NU_HANDLE_GET_ID(handle, id); \
    (id & NUECS_ENTITY_VERSION_MASK) >> 24;})
#define NUECS_ENTITY_ID(handle) ({ \
    uint32_t id; NU_HANDLE_GET_ID(handle, id); \
    (id & NUECS_ENTITY_ID_MASK) >> 16;})
#define NUECS_ENTITY_CHUNK(handle) ({ \
    uint32_t id; NU_HANDLE_GET_ID(handle, id); \
    (id & NUECS_ENTITY_CHUNK_MASK);})

#define NUECS_ENTITY_BUILD_HANDLE(version, id, chunk) ({ \
    uint32_t handle_id = ((((uint32_t)version) & 0xFF) << 24) | \
    ((((uint32_t)id) & 0xFF) << 16) | \
    (((uint32_t)chunk) & 0xFFFF); \
    nuecs_entity_t handle; \
    NU_HANDLE_SET_ID(handle, handle_id); \
    handle;})

typedef struct {
    uint32_t reference_index;
    uint8_t id_to_index;
    uint8_t index_to_id;
    uint8_t version;
} nuecs_chunk_row_t;

typedef struct nuecs_chunk_data {
    nuecs_archetype_data_t *archetype;
    uint16_t id;
    uint8_t size;
    uint8_t frame_size;
    uint8_t free_count;
    uint8_t next_version;
    nuecs_chunk_row_t *rows;
    nuecs_component_data_ptr_t *component_list_ptrs;
} nuecs_chunk_data_t;

nu_result_t nuecs_chunk_allocate(
    nuecs_archetype_data_t *archetype,
    nuecs_chunk_data_t **chunk
);
nu_result_t nuecs_chunk_free(nuecs_chunk_data_t *chunk);
nu_result_t nuecs_chunk_add(
    nuecs_chunk_data_t *chunk,
    nuecs_entity_t *handle
);
nu_result_t nuecs_chunk_remove(
    nuecs_chunk_data_t *chunk,
    nuecs_entity_t handle
);
nu_result_t nuecs_chunk_transfer(
    nuecs_chunk_data_t *src,
    nuecs_chunk_data_t *dst,
    nuecs_entity_t *handle
);
nu_result_t nuecs_chunk_get_component(
    nuecs_chunk_data_t *chunk,
    nuecs_entity_t handle,
    uint32_t component_index,
    nuecs_component_data_ptr_t *component
);
nu_result_t nuecs_chunk_write_component(
    nuecs_chunk_data_t *chunk,
    nuecs_entity_t handle,
    uint32_t component_index,
    nuecs_component_data_ptr_t data
);
bool nuecs_chunk_check_entity(
    nuecs_chunk_data_t *chunk,
    nuecs_entity_t handle
);
nu_result_t nuecs_chunk_get_entity(
    nuecs_chunk_data_t *chunk,
    uint8_t index,
    nuecs_entity_t *handle
);
nu_result_t nuecs_chunk_set_entity_reference_index(
    nuecs_chunk_data_t *chunk,
    nuecs_entity_t handle,
    uint32_t index
);
nu_result_t nuecs_chunk_get_entity_reference_index(
    nuecs_chunk_data_t *chunk,
    nuecs_entity_t handle,
    uint32_t *index
);

#endif