#include <nucleus/module/vulkan/renderer.h>

#include <nucleus/module/vulkan/glfw/interface.h>
#include <nucleus/module/vulkan/core/context.h>
#include <nucleus/module/vulkan/core/memory_manager.h>
#include <nucleus/module/vulkan/core/command_pool.h>
#include <nucleus/module/vulkan/core/swapchain.h>
#include <nucleus/module/vulkan/core/render_context.h>

#define NUVK_LOGGER_NAME "[VULKAN] "

typedef struct {
    nuvk_context_t context;
    nuvk_memory_manager_t memory_manager;
    nuvk_command_pool_t command_pool;
    nuvk_swapchain_t swapchain;
    nuvk_render_context_t render_context;

    bool render_context_out_of_date;
} nuvk_module_data_t;

static nuvk_module_data_t _module;

nu_result_t nuvk_renderer_initialize(void)
{
    if (nuvk_glfw_load_interface() != NU_SUCCESS) {
        nu_error(NUVK_LOGGER_NAME"Failed to load glfw interface.\n");
        return NU_FAILURE;
    }

    if (nuvk_context_initialize(&_module.context) != NU_SUCCESS) {
        return NU_FAILURE;
    }
    if (nuvk_memory_manager_initialize(&_module.memory_manager, &_module.context) != NU_SUCCESS) {
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

    _module.render_context_out_of_date = false;

    return NU_SUCCESS;
}
nu_result_t nuvk_renderer_terminate(void)
{
    nuvk_render_context_terminate(&_module.render_context, &_module.context);
    nuvk_swapchain_terminate(&_module.swapchain, &_module.context);
    nuvk_command_pool_terminate(&_module.command_pool, &_module.context);
    nuvk_memory_manager_terminate(&_module.memory_manager);
    nuvk_context_terminate(&_module.context);

    return NU_SUCCESS;
}

static bool nuvk_renderer_try_render(void)
{
    if (!nuvk_render_context_begin(&_module.render_context, &_module.context, &_module.swapchain)) return false;

    VkCommandBuffer cmd = _module.render_context.command_buffers[_module.render_context.active_inflight_frame_index];

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

            /* reset out of date flag */
            _module.render_context_out_of_date = false;
        }
    }

    return NU_SUCCESS;
}