#ifndef NUGLFW_MODULE_H
#define NUGLFW_MODULE_H

#include <nucleus/module/glfw/module/interface.h>

/* module */
NU_API nu_result_t nu_module_get_info(nu_module_info_t *info);
NU_API nu_result_t nu_module_get_interface(const char *name, void *interface);

#endif