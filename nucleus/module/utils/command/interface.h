#ifndef NUUTILS_COMMAND_INTERFACE_H
#define NUUTILS_COMMAND_INTERFACE_H

#include <nucleus/nucleus.h>

/* command plugin */
#define NUUTILS_COMMAND_PLUGIN_NAME "nuutils_command_plugin"
#define NUUTILS_COMMAND_INTERFACE_NAME "nuutils_command_interface"
#define NUUTILS_COMMAND_INTERFACE_LOADER_NAME "nuutils_command_get_interface"

typedef struct {
    char **args;
    uint32_t argc;
} nuutils_command_event_t;

typedef struct {
    nu_result_t (*get_evend_id)(nu_event_id_t*);
    nu_result_t (*execute)(const char*);
} nuutils_command_interface_t;

typedef nu_result_t (*nuutils_command_interface_loader_pfn_t)(nuutils_command_interface_t*);

#endif