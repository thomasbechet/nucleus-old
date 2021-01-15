#pragma once

#include "../module/interface.h"

/* plugin interface */
nu_result_t nuutils_command_plugin_initialize(void);
nu_result_t nuutils_command_plugin_terminate(void);

/* command interface */
nu_result_t nuutils_command_get_event_id(nu_event_id_t *id);
nu_result_t nuutils_command_execute(const char *command);

extern "C"
{
    NU_API nu_result_t nuutils_command_get_interface(nuutils_command_interface_t *interface);
}