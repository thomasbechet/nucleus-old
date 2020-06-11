#include "surface.h"

#include "../common/logger.h"
#include "../glfw/interface.h"
#include "instance.h"

typedef struct {
    VkSurfaceKHR surface;
} nuvk_surface_data_t;

static nuvk_surface_data_t _data;

nu_result_t nuvk_surface_create(void)
{
    nu_result_t result;
    result = NU_SUCCESS;

    result = nuvk_glfw_create_window_surface(nuvk_instance_get_handle(), &_data.surface);
    if (result != NU_SUCCESS) {
        nu_warning(NUVK_LOGGER_VULKAN"Failed to create window surface.\n");
    }

    return result;
}
nu_result_t nuvk_surface_destroy(void)
{
    vkDestroySurfaceKHR(nuvk_instance_get_handle(), _data.surface, NULL);
    return NU_SUCCESS;
}
VkSurfaceKHR nuvk_surface_get_handle(void)
{
    return _data.surface;
}