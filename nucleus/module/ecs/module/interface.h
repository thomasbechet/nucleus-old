#ifndef NUECS_INTERFACE_H
#define NUECS_INTERFACE_H

#include <nucleus/nucleus.h>

/* module */
#define NUECS_MODULE_NAME "nucleus-ecs"
#define NUECS_MODULE_ID 0x2

/* interface */
#define NUECS_PLUGIN_NAME "nuecs_plugin"
#define NUECS_PLUGIN_INTERFACE_NAME "nuecs_plugin_interface"

#define NUECS_MAX_COMPONENT_PER_ENTITY 64

NU_DECLARE_HANDLE(nuecs_world_t);
NU_DECLARE_HANDLE(nuecs_entity_t);
NU_DECLARE_HANDLE(nuecs_component_t);
NU_DECLARE_HANDLE(nuecs_system_t);

typedef void *nuecs_component_data_ptr_t;

typedef struct {
    nuecs_component_data_ptr_t *components;
    uint32_t count;
} nuecs_chunk_t;

typedef struct {
    const char *name;
    uint32_t size;
} nuecs_component_info_t;

typedef struct {
    nuecs_component_t *components;
    uint32_t component_count;
    nu_result_t (*update)(const nuecs_chunk_t*);
} nuecs_system_info_t;

typedef struct {
    nuecs_component_t *components;
    nuecs_component_data_ptr_t *data;
    uint32_t component_count;
} nuecs_entity_info_t;

typedef struct {
    nu_result_t (*world_create)(nuecs_world_t*);
    nu_result_t (*world_update)(nuecs_world_t);
    nu_result_t (*component_register)(nuecs_world_t, const nuecs_component_info_t*, nuecs_component_t*);
    nu_result_t (*system_register)(nuecs_world_t, const nuecs_system_info_t*, nuecs_system_t*);
    nu_result_t (*entity_create)(nuecs_world_t, const nuecs_entity_info_t*, nuecs_entity_t*);
    nu_result_t (*entity_destroy)(nuecs_world_t, nuecs_entity_t);
} nuecs_plugin_interface_t;

#define NUECS_REGISTER_COMPONENT(ecs, world, component, handle) \
    { \
        nuecs_component_info_t handle##_info; \
        handle##_info.name = #component; \
        handle##_info.size = sizeof(component); \
        ecs.component_register(world, &handle##_info, &handle); \
    } 

#define NUECS_CREATE_ENTITY(ecs, world, component_list, data_list, handle) \
    { \
        nuecs_entity_info_t handle##_info; \
        handle##_info.components = component_list; \
        handle##_info.component_count = sizeof(component_list) / sizeof(nuecs_component_t); \
        handle##_info.data = data_list; \
        ecs.entity_create(world, &handle##_info, &handle); \
    }

#endif