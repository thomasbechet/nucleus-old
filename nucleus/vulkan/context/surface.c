#include "surface.h"

#include "../common/logger.h"
#include "../glfw/interface.h"

nu_result_t nuvk_surface_create(VkSurfaceKHR *surface, VkInstance instance)
{
    nu_result_t result;
    result = NU_SUCCESS;

    result = nuvk_glfw_create_window_surface(instance, surface);
    if (result != NU_SUCCESS) {
        nu_warning(NUVK_VULKAN_LOG_NAME"Failed to create window surface.\n");
    }

    return result;
}
nu_result_t nuvk_surface_destroy(VkSurfaceKHR surface, VkInstance instance)
{
    vkDestroySurfaceKHR(instance, surface, NULL);
    return NU_SUCCESS;
}