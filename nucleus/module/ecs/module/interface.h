#ifndef NUECS_INTERFACE_H
#define NUECS_INTERFACE_H

#include <nucleus/nucleus.h>

/* module */
#define NUECS_MODULE_NAME "nucleus-ecs"
#define NUECS_MODULE_ID 0x2

/* interface */
#define NUECS_INTERFACE_NAME "nuecs_interface"

NU_DECLARE_HANDLE(nuecs_world_t);
NU_DECLARE_HANDLE(nuecs_entity_t);
NU_DECLARE_HANDLE(nuecs_component_t);
NU_DECLARE_HANDLE(nuecs_system_t);

typedef struct {
    const char *name;
    uint32_t size;
} nuecs_component_info_t;

typedef struct {
    nuecs_component_t *components;
    uint32_t component_count;
} nuecs_system_info_t;

typedef struct {
    nu_result_t (*world_create)(nuecs_world_t*);
    nu_result_t (*world_update)(nuecs_world_t);
    nu_result_t (*component_register)(nuecs_world_t, nuecs_component_t*);
    nu_result_t (*system_register)(nuecs_world_t, nuecs_system_t*);
} nuecs_interface_t;

#endif