#include <nucleus/module/vulkan/renderer.h>

#include <nucleus/module/vulkan/glfw/interface.h>
#include <nucleus/module/vulkan/core/allocator.h>
#include <nucleus/module/vulkan/core/context.h>
#include <nucleus/module/vulkan/core/memory_manager.h>
#include <nucleus/module/vulkan/core/command_pool.h>
#include <nucleus/module/vulkan/core/swapchain.h>

#define NUVK_LOGGER_NAME "[VULKAN] "

typedef struct {
    VkAllocationCallbacks allocator;
    nuvk_context_t context;
    nuvk_memory_manager_t memory_manager;
    nuvk_command_pool_t command_pool;
    nuvk_swapchain_t swapchain;

    bool render_context_out_of_date;
} nuvk_module_data_t;

static nuvk_module_data_t _module;

nu_result_t nuvk_renderer_initialize(void)
{
    if (nuvk_glfw_load_interface() != NU_SUCCESS) {
        nu_error(NUVK_LOGGER_NAME"Failed to load glfw interface.\n");
        return NU_FAILURE;
    }

    if (nuvk_allocator_initialize(&_module.allocator) != NU_SUCCESS) {
        return NU_FAILURE;
    }
    if (nuvk_context_initialize(&_module.context, &_module.allocator) != NU_SUCCESS) {
        return NU_FAILURE;
    }
    if (nuvk_memory_manager_initialize(&_module.memory_manager, &_module.context, &_module.allocator) != NU_SUCCESS) {
        return NU_FAILURE;
    }
    if (nuvk_command_pool_initialize(&_module.command_pool, &_module.context, &_module.allocator) != NU_SUCCESS) {
        return NU_FAILURE;
    }
    if (nuvk_swapchain_initialize(&_module.swapchain, &_module.context, &_module.allocator, 1600, 900) != NU_SUCCESS) {
        return NU_FAILURE;
    }

    _module.render_context_out_of_date = false;

    return NU_SUCCESS;
}
nu_result_t nuvk_renderer_terminate(void)
{
    nuvk_swapchain_terminate(&_module.swapchain, &_module.context, &_module.allocator);
    nuvk_command_pool_terminate(&_module.command_pool, &_module.context, &_module.allocator);
    nuvk_memory_manager_terminate(&_module.memory_manager);
    nuvk_context_terminate(&_module.context, &_module.allocator);

    return NU_SUCCESS;
}

static bool nuvk_renderer_try_render(void)
{

}
nu_result_t nuvk_renderer_render(void)
{


    return NU_SUCCESS;
}