#include "interface.h"

nu_result_t nu_window_interface_get_required_functions(const char ***functions, uint32_t *count)
{
    static uint32_t nu_window_interface_function_count = 4;
    static const char *nu_window_interface_functions[] = {
        "nu_window_initialize",
        "nu_window_terminate",
        "nu_window_update",
        "nu_window_get_size"
    };

    *functions = nu_window_interface_functions;
    *count = nu_window_interface_function_count;
    return NU_SUCCESS;
}

nu_result_t nu_renderer_interface_get_required_functions(const char ***functions, uint32_t *count)
{
    static uint32_t nu_renderer_interface_function_count = 3;
    static const char *nu_renderer_interface_functions[] = {
        "nu_renderer_initialize",
        "nu_renderer_terminate",
        "nu_renderer_render"
    };

    *functions = nu_renderer_interface_functions;
    *count = nu_renderer_interface_function_count;
    return NU_SUCCESS;
}