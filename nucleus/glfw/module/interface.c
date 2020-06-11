#include "interface.h"

nu_result_t nuglfw_window_interface_get_required_functions(const char ***functions, uint32_t *count)
{
    static const uint32_t nuglfw_window_interface_function_count = 2;
    static const char *nuglfw_window_interface_functions[] = {
        "nuglfw_get_required_instance_extensions",
        "nuglfw_create_window_surface"
    };

    *count = nuglfw_window_interface_function_count;
    *functions = nuglfw_window_interface_functions;
    return NU_SUCCESS;
}