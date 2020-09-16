#pragma once

#include "../../../core/nucleus.h"
#include "../module/interface.h"

/* plugin interface */
nu_result_t nuutils_plugin_command_initialize(void);
nu_result_t nuutils_plugin_command_terminate(void);

/* command interface */
nu_result_t nuutils_command_get_event_id(nu_event_id_t *id);
nu_result_t nuutils_command_post_command(const char *command);

extern "C"
{
    NU_API nu_result_t nuutils_command_get_interface(nuutils_command_interface_t *interface);
}