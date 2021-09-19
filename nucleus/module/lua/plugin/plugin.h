#ifndef NULUA_PLUGIN_H
#define NULUA_PLUGIN_H

#include <nucleus/module/lua/module/interface.h>

/* plugin interface */
nu_result_t nulua_plugin_initialize(void);
nu_result_t nulua_plugin_terminate(void);
nu_result_t nulua_plugin_update(void);

/* lua plugin interface */
nu_result_t nulua_plugin_load(const char *filename, nulua_plugin_t *handle);

#endif