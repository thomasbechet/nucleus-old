/* Generated file: DO NOT EDIT ! */
#ifndef NULUA_IMPLEMENTATION_H
#define NULUA_IMPLEMENTATION_H

#include <nucleus/nucleus.h>
#include <nucleus/module/lua/plugin/types_public.h>

/* plugin interface */
nu_result_t nulua_plugin_load_impl(const char* path, nulua_plugin_t* handle);
/* manager plugin */
nu_result_t nulua_manager_plugin_initialize_impl(void);
nu_result_t nulua_manager_plugin_terminate_impl(void);
nu_result_t nulua_manager_plugin_update_impl(void);

#endif