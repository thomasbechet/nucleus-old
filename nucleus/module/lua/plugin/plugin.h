#ifndef NULUA_PLUGIN_H
#define NULUA_PLUGIN_H

#include <nucleus/nucleus.h>

/* plugin interface */
nu_result_t nulua_plugin_initialize(void);
nu_result_t nulua_plugin_terminate(void);
nu_result_t nulua_plugin_update(void);

#endif