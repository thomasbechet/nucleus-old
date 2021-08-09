#include <nucleus/module/vulkan/sdf/shader/shaders.h>

nu_result_t nuvk_sdf_shaders_initialize(
    nuvk_sdf_shaders_t *shaders,
    const nuvk_context_t *context,
    const nuvk_shader_manager_t *shader_manager
)
{
    nu_result_t result = NU_SUCCESS;

    result &= nuvk_sdf_shader_postprocess_create(&shaders->postprocess, context, shader_manager);

    return result;
}
nu_result_t nuvk_sdf_shaders_terminate(
    nuvk_sdf_shaders_t *shaders,
    const nuvk_context_t *context
)
{
    nu_result_t result = NU_SUCCESS;

    result &= nuvk_sdf_shader_postprocess_destroy(&shaders->postprocess, context);

    return result;
}