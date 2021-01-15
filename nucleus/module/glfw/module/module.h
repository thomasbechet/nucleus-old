#ifndef NUGLFW_MODULE_H
#define NUGLFW_MODULE_H

#include "interface.h"

/* module */
NU_API nu_result_t nu_module_get_info(nu_module_info_t *info);

/* interface */
NU_API nu_result_t nu_window_get_interface(nu_window_interface_t *interface);
NU_API nu_result_t nu_input_get_interface(nu_input_interface_t *interface);
NU_API nu_result_t nuglfw_window_get_interface(nuglfw_window_interface_t *interface);

#endif