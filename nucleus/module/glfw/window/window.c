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
} nuglfw_module_data_t;

static nuglfw_module_data_t _module;

nu_result_t nuglfw_window_initialize(void)
{
    nu_result_t result;

    GLenum status = glfwInit();
    NU_CHECK(status, return NU_FAILURE, NUGLFW_LOGGER_NAME, "Failed to initialize glew.");

    /* pre context initialization */
    nu_renderer_api_t api = nu_config_get().renderer.api;
    if (api == NU_RENDERER_API_SOFTRAST) {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_SAMPLES, 0);
    } else if (api == NU_RENDERER_API_VULKAN) {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        NU_CHECK(glfwVulkanSupported() == GLFW_TRUE, goto cleanup0, NUGLFW_LOGGER_NAME, "Vulkan not supported...");
    }

    /* create window */
    _module.size[0] = nu_config_get().window.width;
    _module.size[1] = nu_config_get().window.height;
    _module.window = glfwCreateWindow(_module.size[0], _module.size[1], "Window", NULL, NULL);
    NU_CHECK(_module.window, goto cleanup0, NUGLFW_LOGGER_NAME, "Failed to create glfw window.");
    nuglfw_window_set_mode(nu_config_get().window.mode);

    /* post context initialization */
    if (api == NU_RENDERER_API_SOFTRAST) {
        glfwMakeContextCurrent(_module.window);
        result = nuglfw_surface_create();
        NU_CHECK(result == NU_SUCCESS, goto cleanup0, NUGLFW_LOGGER_NAME, "Failed to create surface.");
    }

    if (nu_config_get().window.vsync) {
        glfwSwapInterval(1);
    } else {
        glfwSwapInterval(0);
    }

    return NU_SUCCESS;

cleanup0:
    glfwTerminate();

    return NU_FAILURE;
}
nu_result_t nuglfw_window_terminate(void)
{
    if (nu_config_get().renderer.api == NU_RENDERER_API_SOFTRAST) {
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
    if (glfwWindowShouldClose(_module.window)) {
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
    nu_vec2u_copy(size, _module.size);
    glfwSetWindowSize(_module.window, size[0], size[1]);
    return NU_SUCCESS;
}
nu_result_t nuglfw_window_get_size(nu_vec2u_t size)
{
    glfwGetWindowSize(_module.window, (int*)&size[0], (int*)&size[1]);
    return NU_SUCCESS;
}
nu_result_t nuglfw_window_set_title(const char *title)
{
    glfwSetWindowTitle(_module.window, title);
    return NU_SUCCESS;
}
nu_result_t nuglfw_window_set_mode(nu_window_mode_t mode)
{
    if (mode == NU_WINDOW_MODE_WINDOWED) {
        glfwSetWindowMonitor(_module.window, NULL, 10, 10, _module.size[0], _module.size[1], 0);
    } else if (mode == NU_WINDOW_MODE_FULLSCREEN) {
        const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        glfwSetWindowMonitor(_module.window, glfwGetPrimaryMonitor(), 0, 0, mode->width, mode->height, mode->refreshRate); 
    } else if (mode == NU_WINDOW_MODE_BORDERLESS) {
        const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        glfwWindowHint(GLFW_RED_BITS, mode->redBits);
        glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
        glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
        glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
        glfwSetWindowMonitor(_module.window, glfwGetPrimaryMonitor(), 0, 0, mode->width, mode->height, mode->refreshRate);
    }

    return NU_SUCCESS;
}

nu_result_t nuglfw_window_get_required_instance_extensions(const char*** extensions, uint32_t* extension_count)
{
    *extensions = glfwGetRequiredInstanceExtensions(extension_count);
    return NU_SUCCESS;
}
nu_result_t nuglfw_window_create_window_surface(void* instance_ptr, void* surface_ptr, const void* allocator_ptr)
{
    #ifdef GLFW_INCLUDE_VULKAN
    VkInstance *instance = (VkInstance*)instance_ptr;
    VkSurfaceKHR *surface = (VkSurfaceKHR*)surface_ptr;
    const VkAllocationCallbacks *allocator = (const VkAllocationCallbacks*)allocator_ptr;

    VkResult result = glfwCreateWindowSurface(*instance, _module.window, allocator, surface);
    NU_CHECK(result == VK_SUCCESS, return NU_FAILURE, NUGLFW_LOGGER_NAME, "Failed to create surface.");

    return NU_SUCCESS;
#else
    return NU_FAILURE;
#endif
}
nu_result_t nuglfw_window_present_surface(const nu_vec2u_t size, void* pixels)
{
    int32_t w, h;
    glfwGetWindowSize(_module.window, &w, &h);
    glViewport(0, 0, w, h);
    nuglfw_surface_draw(size[0], size[1], pixels);
    glfwSwapBuffers(_module.window);
    return NU_SUCCESS;
}
nu_result_t nuglfw_window_swap_buffers(void)
{
    glfwSwapBuffers(_module.window);
    return NU_SUCCESS;
}

void *nuglfw_get_window(void)
{
    return (void*)_module.window;
}