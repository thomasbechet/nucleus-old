#include <nucleus/module/vulkan/glfw/interface.h>

#include <nucleus/module/glfw.h>

#define NUVK_LOGGER_NAME "[VULKAN GLFW] "

static nuglfw_window_interface_t _glfw_interface;

nu_result_t nuvk_glfw_get_interface(void)
{
    nu_module_t module = nu_window_get_module();
    if (nu_module_get_id(module) != NUGLFW_MODULE_ID) {
        nu_error(NUVK_LOGGER_NAME"Vulkan API requires GLFW module to work.\n");
        return NU_FAILURE;
    }

    if (nu_module_get_interface(module, NUGLFW_WINDOW_INTERFACE_NAME, &_glfw_interface) != NU_SUCCESS) {
        nu_error(NUVK_LOGGER_NAME"Vulkan failed to get glfw loader.\n");
        return NU_FAILURE;
    }

    return NU_SUCCESS;
}
nu_result_t nuvk_glfw_create_window_surface(VkInstance instance, VkSurfaceKHR *surface, const VkAllocationCallbacks *allocator)
{
    return _glfw_interface.create_window_surface(&instance, surface, allocator);
}
nu_result_t nuvk_glfw_get_required_instance_extensions(const char ***required_extensions, uint32_t *count)
{
    *required_extensions = _glfw_interface.get_required_instance_extensions(count);
    return NU_SUCCESS;
}