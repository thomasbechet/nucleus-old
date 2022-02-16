#ifndef NULUA_PLUGIN_H
#define NULUA_PLUGIN_H

#include <nucleus/module/lua/module/interface.h>

/* manager plugin */
nu_result_t nulua_manager_plugin_initialize(void);
nu_result_t nulua_manager_plugin_terminate(void);
nu_result_t nulua_manager_plugin_update(void);

/* manager interface */
nu_result_t nulua_manager_load_plugin(const char* path, nulua_plugin_t* handle);

#endif