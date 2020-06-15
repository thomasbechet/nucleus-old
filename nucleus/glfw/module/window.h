#ifndef NUGLFW_MODULE_WINDOW_H
#define NUGLFW_MODULE_WINDOW_H

#include "../../core/nucleus_core.h"
#include "interface.h"

/* module */
NU_API nu_result_t nu_module_get_info(nu_module_info_t *info);

/* loaders */
NU_API nu_result_t nu_window_interface_loader(nu_window_interface_t *interface);
NU_API nu_result_t nuglfw_window_interface_loader(nuglfw_window_interface_t *interface);

#endif