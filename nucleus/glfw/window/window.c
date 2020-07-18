#include "window.h"

#include <GL/glew.h>

#include "../common/logger.h"
#include "callback.h"
#include "surface.h"
#include "../../vulkan/module/interface.h"

static GLFWwindow *_window;

nu_result_t nuglfw_window_initialize(void)
{
    if (!glfwInit()) {
        nu_warning(NUGLFW_LOGGER_NAME"Failed to initialize glfw.\n");
        return NU_FAILURE;
    }

    /* pre context initialization */
    if (nu_config_get().renderer_api == NU_RENDERER_API_SOFTRAST) {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_SAMPLES, 0);
    } else if (nu_config_get().renderer_api == NU_RENDERER_API_VULKAN) {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        if (!glfwVulkanSupported()) { /* TODO */
            nu_warning(NUGLFW_LOGGER_NAME"Vulkan not supported...\n");
            glfwTerminate();
            return NU_FAILURE;
        }
    }

    //const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    //_window = glfwCreateWindow(mode->width, mode->height, "Window", glfwGetPrimaryMonitor(), NULL);
    _window = glfwCreateWindow(800, 450, "Window", NULL, NULL);
    if (!_window) {
        nu_warning(NUGLFW_LOGGER_NAME"Failed to create glfw window.\n");
        glfwTerminate();
        return NU_FAILURE;
    }

    /* post context initialization */
    if (nu_config_get().renderer_api == NU_RENDERER_API_SOFTRAST) {
        glfwMakeContextCurrent(_window);
        glfwSwapInterval(0);
        if (glewInit()) {
            nu_warning(NUGLFW_LOGGER_NAME"Failed to initialize glew.\n");
            return NU_FAILURE;
        }
        nuglfw_surface_create();
    }

    /* bind callback */
    glfwSetKeyCallback(_window, key_callback);

    return NU_SUCCESS;
}
nu_result_t nuglfw_window_terminate(void)
{
    if (nu_config_get().renderer_api == NU_RENDERER_API_SOFTRAST) {
        nuglfw_surface_destroy();
    }

    glfwTerminate();

    return NU_SUCCESS;
}
nu_result_t nuglfw_window_update(void)
{
    if (glfwWindowShouldClose(_window)) {
        nu_context_request_stop();
    }

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
    // VkInstance *instance = (VkInstance*)instance_ptr;
    // VkSurfaceKHR *surface = (VkSurfaceKHR*)surface_ptr;

    // VkResult result = glfwCreateWindowSurface(*instance, _window, NULL, surface);
    // if (result != VK_SUCCESS) {
    //     nu_warning(NUGLFW_LOGGER_GLFW"Failed to create surface.\n");
    //     return NU_FAILURE; 
    // }

    return NU_SUCCESS;
}
nu_result_t nuglfw_present_surface(
    uint32_t width,
    uint32_t height,
    void *pixels
)
{
    int32_t w, h;
    glfwGetWindowSize(_window, &w, &h);
    glViewport(0, 0, w, h);
    nuglfw_surface_draw(width, height, pixels);
    glfwSwapBuffers(_window);

    return NU_SUCCESS;
}
