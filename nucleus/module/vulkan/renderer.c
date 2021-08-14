#include <nucleus/module/vulkan/renderer.h>

#include <nucleus/module/vulkan/glfw/interface.h>
#include <nucleus/module/vulkan/core/context.h>
#include <nucleus/module/vulkan/core/memory.h>
#include <nucleus/module/vulkan/core/command_pool.h>
#include <nucleus/module/vulkan/core/swapchain.h>
#include <nucleus/module/vulkan/core/render_context.h>
#include <nucleus/module/vulkan/core/shader.h>
#include <nucleus/module/vulkan/sdf/renderer.h>

#define NUVK_LOGGER_NAME "[VULKAN] "

typedef struct {
    nuvk_context_t context;
    nuvk_memory_manager_t memory_manager;
    nuvk_shader_manager_t shader_manager;
    nuvk_command_pool_t command_pool;
    nuvk_swapchain_t swapchain;
    nuvk_render_context_t render_context;

    nuvk_sdf_renderer_t renderer;

    bool render_context_out_of_date;
} nuvk_module_data_t;

static nuvk_module_data_t _module;

nu_result_t nuvk_renderer_initialize(void)
{
    _module.render_context_out_of_date = false;

    if (nuvk_glfw_get_interface() != NU_SUCCESS) {
        nu_error(NUVK_LOGGER_NAME"Failed to get glfw interface.\n");
        return NU_FAILURE;
    }

    if (nuvk_context_initialize(&_module.context) != NU_SUCCESS) {
        return NU_FAILURE;
    }
    if (nuvk_memory_manager_initialize(&_module.memory_manager, &_module.context) != NU_SUCCESS) {
        return NU_FAILURE;
    }
    if (nuvk_shader_manager_initialize(&_module.shader_manager) != NU_SUCCESS) {
        return NU_FAILURE;
    }
    if (nuvk_command_pool_initialize(&_module.command_pool, &_module.context) != NU_SUCCESS) {
        return NU_FAILURE;
    }
    if (nuvk_swapchain_initialize(&_module.swapchain, &_module.context, 1600, 900) != NU_SUCCESS) {
        return NU_FAILURE;
    }
    if (nuvk_render_context_initialize(&_module.render_context, &_module.context, &_module.swapchain, &_module.command_pool, 3) != NU_SUCCESS) {
        return NU_FAILURE;
    }

    if (nuvk_sdf_renderer_initialize(&_module.renderer, &_module.context, &_module.memory_manager, 
        &_module.shader_manager, &_module.swapchain, &_module.render_context) != NU_SUCCESS) {
        return NU_FAILURE;
    }

    return NU_SUCCESS;
}
nu_result_t nuvk_renderer_terminate(void)
{
    nuvk_sdf_renderer_terminate(&_module.renderer, &_module.context, &_module.memory_manager);

    nuvk_render_context_terminate(&_module.render_context, &_module.context);
    nuvk_swapchain_terminate(&_module.swapchain, &_module.context);
    nuvk_command_pool_terminate(&_module.command_pool, &_module.context);
    nuvk_shader_manager_terminate(&_module.shader_manager);
    nuvk_memory_manager_terminate(&_module.memory_manager);
    nuvk_context_terminate(&_module.context);

    return NU_SUCCESS;
}

static bool nuvk_renderer_try_render(void)
{
    if (!nuvk_render_context_begin(&_module.render_context, &_module.context, &_module.swapchain)) return false;

    nuvk_sdf_renderer_render(&_module.renderer, &_module.swapchain, &_module.render_context);

    if (!nuvk_render_context_end(&_module.render_context, &_module.context, &_module.swapchain)) return false;

    return true;
}
nu_result_t nuvk_renderer_render(void)
{
    if (!_module.render_context_out_of_date) {
        _module.render_context_out_of_date = !nuvk_renderer_try_render();
    } else {
        vkDeviceWaitIdle(_module.context.device);
        nu_vec2u_t size;
        nu_window_get_size(size);
        if (size[0] != 0 && size[1] != 0) {
            /* recreate swapchain */
            nuvk_swapchain_recreate(&_module.swapchain, &_module.context, size[0], size[1]);

            /* notify renderer */
            nuvk_sdf_renderer_update_swapchain(&_module.renderer, &_module.context, &_module.swapchain);

            /* reset out of date flag */
            _module.render_context_out_of_date = false;
        }
    }

    return NU_SUCCESS;
}
nu_result_t nuvk_renderer_camera_set_fov(nu_renderer_camera_t handle, float fov)
{
    _module.renderer.camera.fov = fov;

    return NU_SUCCESS;
}
nu_result_t nuvk_renderer_camera_set_view(nu_renderer_camera_t handle, const nu_vec3f_t eye, const nu_vec3f_t forward, const nu_vec3f_t up)
{
    nu_vec3f_copy(eye, _module.renderer.camera.eye);
    nu_vec3f_copy(eye, _module.renderer.camera.center);
    nu_vec3f_add(_module.renderer.camera.center, forward, _module.renderer.camera.center);
    nu_vec3f_copy(up, _module.renderer.camera.up);

    return NU_SUCCESS;
}