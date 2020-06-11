#ifndef NUGLFW_MODULE_WINDOW_H
#define NUGLFW_MODULE_WINDOW_H

#include "../../core/nucleus_core.h"

NU_API nu_result_t nu_module_get_info(nu_module_info_t *info);
NU_API nu_result_t nu_module_get_available_interface(const char ***interfaces, uint32_t *count);
NU_API nu_result_t nu_module_get_interface_required_functions(
    const char *interface_name,
    const char ***functions,
    uint32_t *count
);

NU_API nu_result_t nu_window_initialize(void);
NU_API nu_result_t nu_window_terminate(void);
NU_API nu_result_t nu_window_update(void);
NU_API nu_result_t nu_window_get_size(uint32_t *width, uint32_t *height);

NU_API const char **nuglfw_get_required_instance_extensions(uint32_t *count);
NU_API nu_result_t nuglfw_create_window_surface(nu_ptr_t instance, nu_ptr_t surface);

#endif