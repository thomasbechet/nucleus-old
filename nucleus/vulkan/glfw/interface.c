#include "interface.h"

#include "../common/logger.h"

#include "../../core/context/context.h"
#include "../../glfw/module/interface.h"

static nuglfw_window_interface_t glfw_interface;

nu_result_t nuvk_glfw_load_interface(void)
{
    const nu_module_t *window_module = nu_system_window_get_module();
    if (window_module->info.id != NUGLFW_MODULE_ID) {
        nu_fatal(NUVK_LOGGER_VULKAN"Vulkan API requires GLFW module to work.\n");
        return NU_FAILURE;
    }

    nuglfw_window_interface_loader_pfn_t load_interface;
    if (nu_module_load_function(window_module, NUGLFW_WINDOW_INTERFACE_LOADER_NAME, (nu_pfn_t*)&load_interface) != NU_SUCCESS) {
        nu_warning(NUVK_LOGGER_VULKAN"Vulkan failed to load glfw loader.\n");
        return NU_FAILURE;
    }

    if (load_interface(&glfw_interface) != NU_SUCCESS) {
        nu_warning(NUVK_LOGGER_VULKAN"Vulkan failed to load glfw interface.\n");
        return NU_FAILURE;
    }

    return NU_SUCCESS;
}
nu_result_t nuvk_glfw_create_window_surface(VkInstance instance, VkSurfaceKHR *surface)
{
    return glfw_interface.create_window_surface(&instance, surface);
}
nu_result_t nuvk_glfw_get_required_instance_extensions(const char ***required_extensions, uint32_t *count)
{
    *required_extensions = glfw_interface.get_required_instance_extensions(count);

    return NU_SUCCESS;
}
