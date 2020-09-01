#ifndef NUDEBUG_MODULE_H
#define NUDEBUG_MODULE_H

#include "../../../core/nucleus.h"

/* module */
NU_API nu_result_t nu_module_get_info(nu_module_info_t *info);

/* interface */
NU_API nu_result_t nu_plugin_get_interface(nu_plugin_interface_t *interface, const char *name);

#endif