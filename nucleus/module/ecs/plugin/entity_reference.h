#ifndef NUECS_ENTITY_REFERENCE_H
#define NUECS_ENTITY_REFERENCE_H

#include <nucleus/module/ecs/plugin/chunk.h>
#include <nucleus/module/ecs/plugin/component_manager.h>
#include <nucleus/module/ecs/plugin/types_public.h>
#include <nucleus/module/ecs/plugin/scene.h>

#define NUECS_ENTITY_REFERENCE_VERSION_MASK 0xFF000000
#define NUECS_ENTITY_REFERENCE_INDEX_MASK   0x00FFFFFF

#define NUECS_ENTITY_REFERENCE_VERSION(handle) ({ \
    uint32_t id; NU_HANDLE_GET_ID(handle, id); \
    (id & NUECS_ENTITY_REFERENCE_VERSION_MASK) >> 24;})
#define NUECS_ENTITY_REFERENCE_INDEX(handle) ({ \
    uint32_t id; NU_HANDLE_GET_ID(handle, id); \
    (id & NUECS_ENTITY_REFERENCE_INDEX_MASK);})

#define NUECS_ENTITY_REFERENCE_BUILD_HANDLE(version, index) ({ \
    uint32_t handle_id = ((((uint32_t)version) & 0xFF) << 24) | \
    (((uint32_t)index) & 0xFFFFFF); \
    nuecs_entity_t handle; \
    NU_HANDLE_SET_ID(handle, handle_id); \
    (nuecs_entity_reference_t)handle;})

#define NUECS_ENTITY_REFERENCE_NONE 0xFFFFFFFF

typedef struct {
    nuecs_scene_data_t *scene;
    nu_array_t entities;
} nuecs_serialization_context_data_t;

typedef struct {
    nuecs_scene_data_t *scene;
    nuecs_entity_t *entities;
    uint32_t entity_count;
} nuecs_deserialization_context_data_t;

typedef struct {
    uint32_t *remap;
} nuecs_transfer_context_data_t;

typedef struct {
    nuecs_entity_t entity;
    uint8_t version;
} nuecs_entity_reference_data_t;

nu_result_t nuecs_entity_reference_bind(
    nuecs_scene_data_t *scene,
    nuecs_entity_reference_t *handle,
    nuecs_entity_t entity
);
nu_result_t nuecs_entity_reference_resolve(
    nuecs_scene_data_t *scene,
    nuecs_entity_reference_t *handle,
    nuecs_entity_t *entity
);
nu_result_t nuecs_entity_reference_serialize_json_object(
    nuecs_entity_reference_t *handle, 
    nuecs_serialization_context_t context, 
    nu_json_object_t object, 
    const char* name
);
nu_result_t nuecs_entity_reference_deserialize_json_object(
    nuecs_deserialization_context_t context, 
    nu_json_object_t object, 
    const char* name, 
    nuecs_entity_reference_t* handle
);
nu_result_t nuecs_entity_reference_remap(
    nuecs_transfer_context_t context, 
    nuecs_entity_reference_t* handle
);

#endif