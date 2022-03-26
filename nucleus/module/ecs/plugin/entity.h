#ifndef NUECS_ENTITY_H
#define NUECS_ENTITY_H

#include <nucleus/module/ecs/plugin/chunk.h>
#include <nucleus/module/ecs/plugin/component_manager.h>
#include <nucleus/module/ecs/plugin/types_public.h>
#include <nucleus/module/ecs/plugin/scene.h>

#define NUECS_ENTITY_HANDLE_GET_INDEX(handle) handle & 0x00FFFFFF       /* 24 bits */
#define NUECS_ENTITY_HANDLE_GET_VERSION(handle) (handle >> 24) & 0xFF /*  8 bits */
#define NUECS_ENTITY_HANDLE_BUILD(index, version) ((((uint32_t)version << 24) & 0xFF000000) | (index & 0x00FFFFFF))

typedef struct {
    nuecs_chunk_data_t *chunk; /* NULL if not used */
    uint32_t chunk_id;
    uint8_t version;
} nuecs_entity_data_t;

nu_result_t nuecs_entity_create(
    nuecs_component_manager_data_t *manager,
    nuecs_scene_data_t *scene, 
    const nuecs_entity_info_t* info, 
    nuecs_entity_t* handle
);
nu_result_t nuecs_entity_destroy(
    nuecs_scene_data_t *scene, 
    nuecs_entity_t handle
);
nu_result_t nuecs_entity_add_component(
    nuecs_scene_data_t *scene, 
    nuecs_entity_t handle,
    nuecs_component_t component, 
    nuecs_component_data_ptr_t component_data
);
nu_result_t nuecs_entity_remove_component(
    nuecs_scene_data_t *scene, 
    nuecs_entity_t handle, 
    nuecs_component_t component
);

#endif