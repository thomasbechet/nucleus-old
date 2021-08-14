#include <nucleus/module/vulkan/sdf/buffer/buffers.h>

nu_result_t nuvk_sdf_buffers_initialize(
    nuvk_sdf_buffers_t *buffers,
    const nuvk_context_t *context,
    const nuvk_memory_manager_t *memory_manager,
    const nuvk_render_context_t *render_context
)
{
    nu_result_t result = NU_SUCCESS;

    result &= nuvk_sdf_buffer_environment_create(&buffers->environment, context, memory_manager, render_context);

    return result;
}
nu_result_t nuvk_sdf_buffers_terminate(
    nuvk_sdf_buffers_t *buffers,
    const nuvk_context_t *context,
    const nuvk_memory_manager_t *memory_manager
)
{
    nuvk_sdf_buffer_environment_destroy(&buffers->environment, context, memory_manager);

    return NU_SUCCESS;
}