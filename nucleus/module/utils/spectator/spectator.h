#ifndef NUUTILS_SPECTATOR_H
#define NUUTILS_SPECTATOR_H

#include <nucleus/nucleus.h>

/* plugin interface */
nu_result_t nuutils_spectator_plugin_initialize(void);
nu_result_t nuutils_spectator_plugin_terminate(void);
nu_result_t nuutils_spectator_plugin_update(void);

#endif