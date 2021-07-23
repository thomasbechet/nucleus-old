#ifndef NUUTILS_MODULE_H
#define NUUTILS_MODULE_H

#include <nucleus/nucleus.h>

/* module */
NU_API nu_result_t nu_module_get_info(nu_module_info_t *info);
NU_API nu_result_t nu_module_get_interface(const char *name, void *interface);
NU_API nu_result_t nu_module_get_plugin(const char *name, nu_plugin_interface_t *plugin);

#endif