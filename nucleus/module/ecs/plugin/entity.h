#ifndef NUECS_ENTITY_H
#define NUECS_ENTITY_H

#include <nucleus/module/ecs/plugin/chunk.h>
#include <nucleus/module/ecs/plugin/component_manager.h>
#include <nucleus/module/ecs/plugin/types_public.h>
#include <nucleus/module/ecs/plugin/scene.h>

/* |--VERSION--|--INDEX--| */
/* |   8bits   |  24bits | */
/* max(entity) = 1048576   */
#define NUECS_ENTITY_VERSION_MASK 0xFF000000
#define NUECS_ENTITY_INDEX_MASK   0xFFFFFF
#define NUECS_MAX_ENTITY_COUNT    16777215
#define NUECS_ENTITY_GET_VERSION(handle) ({uint32_t id; NU_HANDLE_GET_ID(handle, id); (id & NUECS_ENTITY_VERSION_MASK) >> 24;})
#define NUECS_ENTITY_GET_INDEX(handle) ({uint32_t id; NU_HANDLE_GET_ID(handle, id); id & NUECS_ENTITY_INDEX_MASK;})
#define NUECS_ENTITY_BUILD_ID(version, index) ( \
    ((((uint32_t)version) & 0xFF) << 24) | \
    (((uint32_t)index) & 0xFFFFFF) \
)

typedef struct {
    nuecs_chunk_data_t *chunk; /* NULL if not used */
    uint32_t chunk_id;
    uint16_t version;
    uint32_t index;
} nuecs_entity_data_t;

typedef struct {
    nuecs_scene_data_t *scene;
    uint32_t *remap;
} nuecs_serialization_context_data_t;

typedef struct {
    nuecs_scene_data_t *scene;
    nuecs_entity_t *handles;
    uint32_t handle_count;
} nuecs_deserialization_context_data_t;

typedef struct {
    uint32_t *remap;
} nuecs_transfer_context_data_t;

nu_result_t nuecs_entity_create(
    nuecs_component_manager_data_t *manager,
    nuecs_scene_data_t *scene, 
    const nuecs_entity_info_t* info, 
    nuecs_entity_t *handle
);
nu_result_t nuecs_entity_destroy(
    nuecs_scene_data_t *scene, 
    nuecs_entity_t handle
);
nu_result_t nuecs_entity_add_entry(
    nuecs_component_manager_data_t *manager,
    nuecs_scene_data_t *scene,
    uint32_t *components,
    uint32_t component_count,
    uint32_t *out_index,
    nuecs_entity_data_t **out_entity
);
nu_result_t nuecs_entity_remove_entry(
    nuecs_scene_data_t *scene,
    uint32_t index
);
nu_result_t nuecs_entity_add_component(
    nuecs_component_manager_data_t *manager,
    nuecs_scene_data_t *scene, 
    nuecs_entity_t handle, 
    nuecs_component_t component, 
    nuecs_component_data_ptr_t data
);
nu_result_t nuecs_entity_remove_component(
    nuecs_component_manager_data_t *manager,
    nuecs_scene_data_t *scene, 
    nuecs_entity_t handle, 
    nuecs_component_t component
);
nu_result_t nuecs_entity_serialize_json_object(
    nuecs_entity_t handle, 
    nuecs_serialization_context_t context, 
    nu_json_object_t object, 
    const char* name
);
nu_result_t nuecs_entity_deserialize_json_object(
    nuecs_deserialization_context_t context, 
    nu_json_object_t object, 
    const char* name, 
    nuecs_entity_t* handle
);
nu_result_t nuecs_entity_remap(
    nuecs_transfer_context_t context, 
    nuecs_entity_t* handle
);

#endif