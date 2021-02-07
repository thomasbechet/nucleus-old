#ifndef NUUTILS_COMMAND_INTERFACE_H
#define NUUTILS_COMMAND_INTERFACE_H

#include <nucleus/nucleus.h>

#define NUUTILS_COMMAND_PLUGIN_NAME "nuutils_command_plugin"
#define NUUTILS_COMMAND_INTERFACE_NAME "nuutils_command_interface"

typedef struct {
    char **args;
    uint32_t argc;
} nuutils_command_event_t;

typedef struct {
    nu_result_t (*get_event_id)(nu_event_id_t*);
    nu_result_t (*execute)(const char*);
} nuutils_command_interface_t;

#endif