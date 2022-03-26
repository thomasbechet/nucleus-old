/* Generated file: DO NOT EDIT ! */
#ifndef NUUTILS_IMPLEMENTATION_H
#define NUUTILS_IMPLEMENTATION_H

#include <nucleus/nucleus.h>
#include <nucleus/module/utils/command/types_public.h>

/* loader interface */
nu_result_t nuutils_loader_load_mesh_from_obj_impl(const char* path, nu_renderer_mesh_t* handle);
nu_result_t nuutils_loader_load_texture_impl(const char* path, nu_renderer_texture_t* handle);
/* command interface */
nu_result_t nuutils_command_get_event_id_impl(nu_event_id_t* id);
nu_result_t nuutils_command_execute_impl(const char* command);
/* command plugin */
nu_result_t nuutils_command_plugin_initialize_impl(void);
nu_result_t nuutils_command_plugin_terminate_impl(void);
nu_result_t nuutils_command_plugin_update_impl(void);

#endif