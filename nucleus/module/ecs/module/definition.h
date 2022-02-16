/* Generated file: DO NOT EDIT ! */
#ifndef NUECS_MODULE_DEFINITION_H
#define NUECS_MODULE_DEFINITION_H

#include <nucleus/nucleus.h>
#include <nucleus/module/ecs/plugin/types_public.h>

/* module */
#define NUECS_MODULE_NAME "nucleus-ecs"
#define NUECS_MODULE_ID 14

/* interface */
#define NUECS_WORLD_INTERFACE_NAME "nuecs_world_interface"

typedef struct {
    nu_result_t (*create)(nuecs_world_t*);
    nu_result_t (*destroy)(nuecs_world_t);
    nu_result_t (*progress)(nuecs_world_t);
    nu_result_t (*register_component)(nuecs_world_t, const nuecs_component_info_t*, nuecs_component_t*);
    nu_result_t (*register_system)(nuecs_world_t, const nuecs_system_info_t*, nuecs_system_t*);
    nu_result_t (*create_entity)(nuecs_world_t, const nuecs_entity_info_t*, nuecs_entity_t*);
    nu_result_t (*destroy_entity)(nuecs_world_t, nuecs_entity_t);
    nu_result_t (*entity_add_component)(nuecs_world_t, nuecs_entity_t, nuecs_component_t, nuecs_component_data_ptr_t);
    nu_result_t (*entity_remove_component)(nuecs_world_t, nuecs_entity_t, nuecs_component_t);
} nuecs_world_interface_t;


/* plugin */
#define NUECS_WORLD_PLUGIN_NAME "nuecs_world_plugin"

#endif