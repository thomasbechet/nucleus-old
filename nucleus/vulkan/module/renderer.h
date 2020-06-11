#ifndef NUVK_MODULE_RENDERER_H
#define NUVK_MODULE_RENDERER_H

#include "../../core/nucleus_core.h"

NU_API nu_result_t nu_module_get_info(nu_module_info_t *info);
NU_API nu_result_t nu_module_get_available_interface(const char ***interfaces, uint32_t *count);
NU_API nu_result_t nu_module_get_interface_required_functions(
    const char *interface_name,
    const char ***functions,
    uint32_t *count
);

NU_API nu_result_t nu_renderer_initialize(nu_ptr_t ptr);
NU_API nu_result_t nu_renderer_terminate(nu_ptr_t ptr);
NU_API nu_result_t nu_renderer_render(nu_ptr_t ptr);

#endif