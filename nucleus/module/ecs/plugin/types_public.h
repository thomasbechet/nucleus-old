#ifndef NUECS_PUBLIC_DEFINITION_H
#define NUECS_PUBLIC_DEFINITION_H

#include <nucleus/nucleus.h>

#define NUECS_MAX_COMPONENT_PER_ENTITY 64

NU_DECLARE_HANDLE(nuecs_world_t);
NU_DECLARE_HANDLE(nuecs_entity_t);
NU_DECLARE_HANDLE(nuecs_component_t);
NU_DECLARE_HANDLE(nuecs_system_t);
NU_DECLARE_HANDLE(nuecs_archetype_t);

typedef void *nuecs_component_data_ptr_t;

typedef struct {
    const char *name;
    uint32_t size;
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

#define NUECS_REGISTER_COMPONENT(ecs, world, component, handle) \
    { \
        nuecs_component_info_t handle##_info; \
        handle##_info.name = #component; \
        handle##_info.size = sizeof(component); \
        nuecs_world_register_component(world, &handle##_info, &handle); \
    }
#define NUECS_REGISTER_SYSTEM(ecs, world, component_list, update_callback, handle) \
    { \
        nuecs_system_info_t handle##_info; \
        handle##_info.components = component_list; \
        handle##_info.component_count = sizeof(component_list) / sizeof(nuecs_component_t); \
        handle##_info.update = update_callback; \
        nuecs_world_register_system(world, &handle##_info, &handle); \
    }

#endif