#include <nucleus/module/vulkan/glfw/interface.h>

#define NUGLFW_BOOTSTRAP
#include <nucleus/module/glfw.h>

#define NUVK_LOGGER_NAME "VULKAN GLFW"

nu_result_t nuvk_glfw_get_interface(void)
{
    nu_module_t module = nu_window_get_module();
    NU_CHECK(nu_module_get_id(module) == NUGLFW_MODULE_ID, return NU_FAILURE, NUVK_LOGGER_NAME, "Vulkan API requires GLFW module to work.");

    nu_result_t result = nuglfw_window_interface_load(module);
    NU_CHECK(result == NU_SUCCESS, return NU_FAILURE, NUVK_LOGGER_NAME, "Vulkan failed to get glfw loader.");

    return NU_SUCCESS;
}
nu_result_t nuvk_glfw_create_window_surface(VkInstance instance, VkSurfaceKHR *surface, const VkAllocationCallbacks *allocator)
{
    return nuglfw_window_create_window_surface(&instance, surface, allocator);
}
nu_result_t nuvk_glfw_get_required_instance_extensions(const char ***required_extensions, uint32_t *count)
{
    return nuglfw_window_get_required_instance_extensions(required_extensions, count);
}