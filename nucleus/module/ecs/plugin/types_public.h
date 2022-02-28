#ifndef NUECS_PUBLIC_DEFINITION_H
#define NUECS_PUBLIC_DEFINITION_H

#include <nucleus/nucleus.h>

#define NUECS_MAX_COMPONENT_PER_ENTITY 64

NU_DECLARE_HANDLE(nuecs_scene_t);
NU_DECLARE_HANDLE(nuecs_entity_t);
NU_DECLARE_HANDLE(nuecs_component_t);
NU_DECLARE_HANDLE(nuecs_system_t);
NU_DECLARE_HANDLE(nuecs_archetype_t);

typedef void *nuecs_component_data_ptr_t;
typedef nu_result_t (*nuecs_component_serialize_json_pfn_t)(const nuecs_component_data_ptr_t, nu_json_object_t);
typedef nu_result_t (*nuecs_component_deserialize_json_pfn_t)(nu_json_object_t, nuecs_component_data_ptr_t);

typedef struct {
    const char *name;
    uint32_t size;
    nuecs_component_serialize_json_pfn_t serialize_json;
    nuecs_component_deserialize_json_pfn_t deserialize_json;
} nuecs_component_info_t;

typedef struct {
    nuecs_component_t *components;
    uint32_t component_count;
    nu_result_t (*update)(nuecs_component_data_ptr_t *components, uint32_t count);
} nuecs_system_info_t;

typedef struct {
    nuecs_component_t *components;
    nuecs_component_data_ptr_t *component_data;
    uint32_t component_count;
} nuecs_entity_info_t;

#define NUECS_REGISTER_COMPONENT(scene, component, handle) \
    { \
        nuecs_component_info_t handle##_info; \
        handle##_info.name             = #component; \
        handle##_info.size             = sizeof(component); \
        handle##_info.serialize_json   = NULL; \
        handle##_info.deserialize_json = NULL; \
        nuecs_scene_register_component(scene, &handle##_info, &handle); \
    }
#define NUECS_REGISTER_SYSTEM(scene, component_list, update_callback, handle) \
    { \
        nuecs_system_info_t handle##_info; \
        handle##_info.components = component_list; \
        handle##_info.component_count = sizeof(component_list) / sizeof(nuecs_component_t); \
        handle##_info.update = update_callback; \
        nuecs_scene_register_system(scene, &handle##_info, &handle); \
    }

#endif