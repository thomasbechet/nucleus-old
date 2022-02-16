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

typedef struct {
    nu_result_t (*load_mesh_from_obj)(const char*, nu_renderer_mesh_t*);
    nu_result_t (*load_texture)(const char*, nu_renderer_texture_t*);
} nuutils_loader_interface_t;

#define NUUTILS_COMMAND_INTERFACE_NAME "nuutils_command_interface"

typedef struct {
    nu_result_t (*get_event_id)(nu_event_id_t*);
    nu_result_t (*execute)(const char*);
} nuutils_command_interface_t;


/* plugin */
#define NUUTILS_COMMAND_PLUGIN_NAME "nuutils_command_plugin"

#endif