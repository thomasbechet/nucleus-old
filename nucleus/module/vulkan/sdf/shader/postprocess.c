#include <nucleus/module/vulkan/sdf/shader/postprocess.h>

nu_result_t nuvk_sdf_shader_postprocess_create(
    nuvk_sdf_shader_postprocess_t *shader,
    const nuvk_context_t *context,
    const nuvk_shader_manager_t *shader_manager
)
{
    if (nuvk_shader_module_create_from_glsl(context, shader_manager, "$ENGINE_DIR/shader/postprocess.vert", 
        VK_SHADER_STAGE_VERTEX_BIT, &shader->vertex) != NU_SUCCESS) {
        nu_error(NUVK_LOGGER_NAME"Failed to create postprocess vertex shader.\n");
        return NU_SUCCESS;
    }

    if (nuvk_shader_module_create_from_glsl(context, shader_manager, "$ENGINE_DIR/shader/postprocess.frag", 
        VK_SHADER_STAGE_FRAGMENT_BIT, &shader->fragment) != NU_SUCCESS) {
        nu_error(NUVK_LOGGER_NAME"Failed to create postprocess fragment shader.\n");
        return NU_SUCCESS;
    }

    shader->stage_count = 2;

    memset(&shader->stages, 0, sizeof(VkPipelineShaderStageCreateInfo) * shader->stage_count);
    shader->stages[0].sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shader->stages[0].stage  = VK_SHADER_STAGE_VERTEX_BIT;
    shader->stages[0].pName  = "main";
    shader->stages[0].module = shader->vertex;
    shader->stages[1].sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shader->stages[1].stage  = VK_SHADER_STAGE_FRAGMENT_BIT;
    shader->stages[1].pName  = "main";
    shader->stages[1].module = shader->fragment;

    return NU_SUCCESS;
}
nu_result_t nuvk_sdf_shader_postprocess_destroy(
    nuvk_sdf_shader_postprocess_t *shader,
    const nuvk_context_t *context
)
{
    vkDestroyShaderModule(context->device, shader->vertex, &context->allocator);
    vkDestroyShaderModule(context->device, shader->fragment, &context->allocator);

    return NU_SUCCESS;
}