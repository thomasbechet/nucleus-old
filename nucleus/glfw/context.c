#include "context.h"

#include "callback.h"
#include "logger.h"
#include "../vulkan/module/interface.h"

static GLFWwindow *_window;

nu_result_t nuglfw_window_initialize(void)
{
    if (!glfwInit()) {
        nu_warning(NUGLFW_LOGGER_GLFW"Failed to initialize glfw.\n");
        return NU_FAILURE;
    }

    if (!glfwVulkanSupported()) { /* TODO */
        nu_warning(NUGLFW_LOGGER_GLFW"Vulkan not supported...\n");
        glfwTerminate();
        return NU_FAILURE;
    }

    if (nu_config_get().renderer_api == NU_RENDERER_API_VULKAN) {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    }

    _window = glfwCreateWindow(800, 450, "Window", NULL, NULL);
    if (!_window) {
        nu_warning(NUGLFW_LOGGER_GLFW"Failed to create glfw window.\n");
        glfwTerminate();
        return NU_FAILURE;
    }
    glfwSetKeyCallback(_window, key_callback);

    return NU_SUCCESS;
}
nu_result_t nuglfw_window_terminate(void)
{
    glfwTerminate();

    return NU_SUCCESS;
}
nu_result_t nuglfw_window_update(void)
{
    if (glfwWindowShouldClose(_window)) {
        nu_context_request_stop();
    }

    glfwSwapBuffers(_window);
    glfwPollEvents();

    return NU_SUCCESS;
}
nu_result_t nuglfw_window_get_size(uint32_t *width, uint32_t *height)
{
    glfwGetWindowSize(_window, (int*)width, (int*)height);
    return NU_SUCCESS;
}

const char **nuglfw_get_required_instance_extensions(uint32_t *count)
{
    return glfwGetRequiredInstanceExtensions(count);
}
nu_result_t nuglfw_create_window_surface(nu_ptr_t instance_ptr, nu_ptr_t surface_ptr)
{
    VkInstance *instance = (VkInstance*)instance_ptr;
    VkSurfaceKHR *surface = (VkSurfaceKHR*)surface_ptr;

    VkResult result = glfwCreateWindowSurface(*instance, _window, NULL, surface);
    if (result != VK_SUCCESS) {
        nu_warning(NUGLFW_LOGGER_GLFW"Failed to create surface.\n");
        return NU_FAILURE; 
    }

    return NU_SUCCESS;
}