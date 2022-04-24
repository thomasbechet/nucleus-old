/* Generated file: DO NOT EDIT ! */
#ifndef NUUTILS_MODULE_DEFINITION_H
#define NUUTILS_MODULE_DEFINITION_H

#include <nucleus/nucleus.h>
#include <nucleus/module/utils/command/types_public.h>

/* module */
#define NUUTILS_MODULE_NAME "nucleus-utils"
#define NUUTILS_MODULE_ID 6

/* interface */
#define NUUTILS_LOADER_INTERFACE_NAME "nuutils_loader_interface"
typedef nu_result_t (*nuutils_loader_load_mesh_from_obj_pfn_t)(const char* path, nu_renderer_mesh_t* handle);
typedef nu_result_t (*nuutils_loader_load_texture_pfn_t)(const char* path, nu_renderer_texture_t* handle);

typedef struct {
    nuutils_loader_load_mesh_from_obj_pfn_t load_mesh_from_obj;
    nuutils_loader_load_texture_pfn_t load_texture;
} nuutils_loader_interface_t;

#define NUUTILS_COMMAND_INTERFACE_NAME "nuutils_command_interface"
typedef nu_result_t (*nuutils_command_get_event_id_pfn_t)(nu_event_id_t* id);
typedef nu_result_t (*nuutils_command_execute_pfn_t)(const char* command);

typedef struct {
    nuutils_command_get_event_id_pfn_t get_event_id;
    nuutils_command_execute_pfn_t execute;
} nuutils_command_interface_t;


/* plugin */
#define NUUTILS_COMMAND_PLUGIN_NAME "nuutils_command_plugin"

#endif