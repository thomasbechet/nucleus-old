/* Generated file: DO NOT EDIT ! */
#ifndef NUECS_MODULE_DEFINITION_H
#define NUECS_MODULE_DEFINITION_H

#include <nucleus/nucleus.h>
#include <nucleus/module/ecs/plugin/types_public.h>
#include <nucleus/module/ecs/plugin/types_public.h>
#include <nucleus/module/ecs/plugin/types_public.h>

/* module */
#define NUECS_MODULE_NAME "nucleus-ecs"
#define NUECS_MODULE_ID 14

/* interface */
#define NUECS_MANAGER_INTERFACE_NAME "nuecs_manager_interface"

typedef struct {
    nu_result_t (*register_component)(const nuecs_component_info_t*, nuecs_component_t*);
    nu_result_t (*create_scene)(nuecs_scene_t*);
    nu_result_t (*destroy_scene)(nuecs_scene_t);
    nu_result_t (*debug_archetypes)(void);
} nuecs_manager_interface_t;

#define NUECS_SCENE_INTERFACE_NAME "nuecs_scene_interface"

typedef struct {
    nu_result_t (*progress)(nuecs_scene_t);
    nu_result_t (*register_system)(nuecs_scene_t, const nuecs_system_info_t*, nuecs_system_t*);
    nu_result_t (*create_entity)(nuecs_scene_t, const nuecs_entity_info_t*, nuecs_entity_t*);
    nu_result_t (*destroy_entity)(nuecs_scene_t, nuecs_entity_t);
    nu_result_t (*entity_add_component)(nuecs_scene_t, nuecs_entity_t, nuecs_component_t, nuecs_component_data_ptr_t);
    nu_result_t (*entity_remove_component)(nuecs_scene_t, nuecs_entity_t, nuecs_component_t);
    nu_result_t (*save_file)(nuecs_scene_t, const char*);
} nuecs_scene_interface_t;

#define NUECS_QUERY_INTERFACE_NAME "nuecs_query_interface"

typedef struct {
    nu_result_t (*create)(nuecs_scene_t, const nuecs_query_info_t*, nuecs_query_t*);
    nu_result_t (*destroy)(nuecs_scene_t, nuecs_query_t);
    nu_result_t (*resolve_chunks)(nuecs_scene_t, nuecs_query_t, nuecs_query_chunks_t*);
} nuecs_query_interface_t;


/* plugin */
#define NUECS_SCENE_PLUGIN_NAME "nuecs_scene_plugin"

#endif