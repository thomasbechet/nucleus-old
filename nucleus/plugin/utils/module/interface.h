#ifndef NUUTILS_INTERFACE_H
#define NUUTILS_INTERFACE_H

#include "../../../core/nucleus.h"

/* module */
#define NUUTILS_MODULE_NAME "nucleus-utils"
#define NUUTILS_MODULE_ID 0x6

/* console plugin */
#define NUUTILS_PLUGIN_CONSOLE_NAME "nuutils_plugin_console"

/* command plugin */
#define NUUTILS_PLUGIN_COMMAND_NAME "nuutils_plugin_command"
#define NUUTILS_COMMAND_INTERFACE_NAME "nuutils_command_interface"
#define NUUTILS_COMMAND_INTERFACE_LOADER_NAME "nuutils_command_get_interface"

typedef struct {
    
} nuutils_command_event_t;

typedef struct {
    nu_result_t (*get_event_id)(nu_event_id_t*);
    nu_result_t (*execute)(const char*);
} nuutils_command_interface_t;

//nu_result_t (*nuutils_command_interface_loader_pfn_t)(nuutils_command_interface_t*);

#endif