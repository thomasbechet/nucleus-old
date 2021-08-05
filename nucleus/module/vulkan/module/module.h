#ifndef NUVK_MODULE_H
#define NUVK_MODULE_H

#include <nucleus/module/vulkan/module/interface.h>

/* module */
NU_API nu_result_t nu_module_info(nu_module_info_t *info);
NU_API nu_result_t nu_module_interface(const char *name, void *interface);

#endif