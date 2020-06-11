#include "renderer.h"

#include "interface.h"
#include "../renderer.h"

nu_result_t nu_module_get_info(nu_module_info_t *info)
{
    info->type = NU_MODULE_TYPE_RENDERER;
    info->id = NUVK_MODULE_ID;
    return NU_SUCCESS;
}
nu_result_t nu_module_get_available_interface(const char ***interfaces, uint32_t *count)
{
    static const uint32_t available_interface_count = 1;
    static const char *available_interfaces[] = {
        NU_RENDERER_INTERFACE
    };

    *count = available_interface_count;
    *interfaces = available_interfaces;
    return NU_SUCCESS;
}
nu_result_t nu_module_get_interface_required_functions(
    const char *interface_name,
    const char ***functions,
    uint32_t *count
)
{
    if (strcmp(interface_name, NU_RENDERER_INTERFACE) == 0) {
        return nu_renderer_interface_get_required_functions(functions, count);
        return NU_SUCCESS;
    }

    return NU_FAILURE;
}

nu_result_t nu_renderer_initialize(nu_ptr_t ptr)
{
    return nuvk_renderer_initialize();
}
nu_result_t nu_renderer_terminate(nu_ptr_t ptr)
{
    return nuvk_renderer_terminate();
}
nu_result_t nu_renderer_render(nu_ptr_t ptr)
{
    return nuvk_renderer_render();
}