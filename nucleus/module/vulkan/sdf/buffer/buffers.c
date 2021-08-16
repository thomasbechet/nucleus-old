#include <nucleus/module/vulkan/sdf/buffer/buffers.h>

nu_result_t nuvk_sdf_buffers_initialize(
    nuvk_sdf_buffers_t *buffers,
    const nuvk_context_t *context,
    const nuvk_memory_manager_t *memory_manager,
    const nuvk_render_context_t *render_context,
    const nuvk_sdf_primitives_t *primitives
)
{
    nu_result_t result = NU_SUCCESS;

    nuvk_sdf_instance_type_info_t info[1] = {
        primitives->sphere
    };

    result &= nuvk_sdf_buffer_environment_create(&buffers->environment, context, memory_manager, render_context);
    result &= nuvk_sdf_buffer_instances_create(&buffers->instances, context, memory_manager, render_context, info, 1);

    return result;
}
nu_result_t nuvk_sdf_buffers_terminate(
    nuvk_sdf_buffers_t *buffers,
    const nuvk_memory_manager_t *memory_manager
)
{
    nuvk_sdf_buffer_instances_destroy(&buffers->instances, memory_manager);
    nuvk_sdf_buffer_environment_destroy(&buffers->environment, memory_manager);

    return NU_SUCCESS;
}