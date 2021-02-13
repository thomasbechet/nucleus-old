#include <nucleus/module/glfw/window/window.h>

#include <nucleus/module/glfw/window/surface.h>
#include <nucleus/module/glfw/common/logger.h>
#include <nucleus/module/glfw/input/input.h>

#include <GL/glew.h>
#ifdef NU_BUILD_MODULE_VULKAN
    #define GLFW_INCLUDE_VULKAN
#endif
#include <GLFW/glfw3.h>

typedef struct {
    GLFWwindow *window;
    nu_vec2u_t size;
} nuglfw_window_data_t;

static nuglfw_window_data_t _data;

nu_result_t nuglfw_window_initialize(void)
{
    if (!glfwInit()) {
        nu_warning(NUGLFW_LOGGER_NAME"Failed to initialize glfw.\n");
        return NU_FAILURE;
    }

    /* pre context initialization */
    nu_renderer_api_t api = nu_config_get().renderer.api;
    if (api == NU_RENDERER_API_SOFTRAST ||
        api == NU_RENDERER_API_OPENGL || 
        api == NU_RENDERER_API_RAYTRACER) {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_SAMPLES, 0);
    } else if (api == NU_RENDERER_API_VULKAN) {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        if (!glfwVulkanSupported()) { /* TODO */
            nu_warning(NUGLFW_LOGGER_NAME"Vulkan not supported...\n");
            glfwTerminate();
            return NU_FAILURE;
        }
    }

    /* create window */
    _data.size[0] = nu_config_get().window.width;
    _data.size[1] = nu_config_get().window.height;
    _data.window = glfwCreateWindow(_data.size[0], _data.size[1], "Window", NULL, NULL);
    nuglfw_window_set_mode(nu_config_get().window.mode);

    if (!_data.window) {
        nu_warning(NUGLFW_LOGGER_NAME"Failed to create glfw window.\n");
        glfwTerminate();
        return NU_FAILURE;
    }

    /* post context initialization */
    if (api == NU_RENDERER_API_SOFTRAST ||
        api == NU_RENDERER_API_RAYTRACER) {
        glfwMakeContextCurrent(_data.window);
        nuglfw_surface_create();
    } else if (api == NU_RENDERER_API_OPENGL) {
        glfwMakeContextCurrent(_data.window);
    }

    if (nu_config_get().window.vsync) {
        glfwSwapInterval(1);
    } else {
        glfwSwapInterval(0);
    }

    return NU_SUCCESS;
}
nu_result_t nuglfw_window_terminate(void)
{
    if (nu_config_get().renderer.api == NU_RENDERER_API_SOFTRAST ||
        nu_config_get().renderer.api == NU_RENDERER_API_RAYTRACER) {
        nuglfw_surface_destroy();
    }

    glfwTerminate();

    return NU_SUCCESS;
}
nu_result_t nuglfw_window_start(void)
{
    return NU_SUCCESS;
}
nu_result_t nuglfw_window_stop(void)
{
    return NU_SUCCESS;
}
nu_result_t nuglfw_window_update(void)
{
    /* check window exit */
    if (glfwWindowShouldClose(_data.window)) {
        nu_context_request_stop();
    }

    /* event input preupdate */
    nuglfw_input_event_preupdate();

    /* trigger events */
    glfwPollEvents();

    return NU_SUCCESS;
}

nu_result_t nuglfw_window_set_size(const nu_vec2u_t size)
{
    nu_vec2u_copy(size, _data.size);
    glfwSetWindowSize(_data.window, size[0], size[1]);
    return NU_SUCCESS;
}
nu_result_t nuglfw_window_get_size(nu_vec2u_t size)
{
    glfwGetWindowSize(_data.window, (int*)&size[0], (int*)&size[1]);
    return NU_SUCCESS;
}
nu_result_t nuglfw_window_set_title(const char *title)
{
    glfwSetWindowTitle(_data.window, title);
    return NU_SUCCESS;
}
nu_result_t nuglfw_window_set_mode(nu_window_mode_t mode)
{
    if (mode == NU_WINDOW_MODE_FULLSCREEN) {
        const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
            glfwSetWindowMonitor(_data.window, glfwGetPrimaryMonitor(), 0, 0, mode->width, mode->height, 0);
    } else if (mode == NU_WINDOW_MODE_WINDOWED) {
        glfwSetWindowMonitor(_data.window, NULL, 10, 10, _data.size[0], _data.size[1], 0);
    } else if (mode == NU_WINDOW_MODE_BORDERLESS) {
        const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
            glfwWindowHint(GLFW_RED_BITS, mode->redBits);
            glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
            glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
            glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
            glfwSetWindowMonitor(_data.window, glfwGetPrimaryMonitor(), 0, 0, mode->width, mode->height, mode->refreshRate);
    }

    return NU_SUCCESS;
}

const char **nuglfw_get_required_instance_extensions(uint32_t *count)
{
    return glfwGetRequiredInstanceExtensions(count);
}
nu_result_t nuglfw_create_window_surface(nu_ptr_t instance_ptr, nu_ptr_t surface_ptr)
{
#ifdef GLFW_INCLUDE_VULKAN
    VkInstance *instance = (VkInstance*)instance_ptr;
    VkSurfaceKHR *surface = (VkSurfaceKHR*)surface_ptr;

    VkResult result = glfwCreateWindowSurface(*instance, _data.window, NULL, surface);
    if (result != VK_SUCCESS) {
        nu_warning(NUGLFW_LOGGER_NAME"Failed to create surface.\n");
        return NU_FAILURE; 
    }

    return NU_SUCCESS;
#else
    return NU_FAILURE;
#endif
}
nu_result_t nuglfw_present_surface(
    const nu_vec2u_t size,
    void *pixels
)
{
    int32_t w, h;
    glfwGetWindowSize(_data.window, &w, &h);
    glViewport(0, 0, w, h);
    nuglfw_surface_draw(size[0], size[1], pixels);
    glfwSwapBuffers(_data.window);

    return NU_SUCCESS;
}
nu_result_t nuglfw_swap_buffers(void)
{
    glfwSwapBuffers(_data.window);
    return NU_SUCCESS;
}

nu_ptr_t nuglfw_get_window(void)
{
    return (nu_ptr_t)_data.window;
}