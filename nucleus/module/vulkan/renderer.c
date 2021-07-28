#include <nucleus/module/vulkan/renderer.h>

#include <nucleus/module/vulkan/core/allocator.h>
#include <nucleus/module/vulkan/core/context.h>

typedef struct {
    VkAllocationCallbacks allocator;
    nuvk_context_t context;
} nuvk_module_data_t;

static nuvk_module_data_t _module;

nu_result_t nuvk_renderer_initialize(void)
{
    if (nuvk_allocator_initialize(&_module.allocator) != NU_SUCCESS) {
        return NU_FAILURE;
    }
    if (nuvk_context_initialize(&_module.context, &_module.allocator) != NU_SUCCESS) {
        return NU_FAILURE;
    }

    return NU_SUCCESS;
}
nu_result_t nuvk_renderer_terminate(void)
{
    nuvk_context_terminate(&_module.context, &_module.allocator);

    return NU_SUCCESS;
}
nu_result_t nuvk_renderer_render(void)
{
    return NU_SUCCESS;
}