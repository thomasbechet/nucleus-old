#include <nucleus/module/vulkan/sdf/shader/postprocess.h>

nu_result_t nuvk_sdf_shader_postprocess_create(
    nuvk_sdf_shader_postprocess_t *shader,
    const nuvk_context_t *context,
    const nuvk_shader_manager_t *shader_manager,
    const nuvk_sdf_descriptors_t *descriptors
)
{
    /* create shader modules */
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

    /* create pipeline layout */
    VkDescriptorSetLayout setLayouts[2] = {
        descriptors->low_frequency.layout,
        descriptors->postprocess.layout
    };

    VkPipelineLayoutCreateInfo layout_info;
    memset(&layout_info, 0, sizeof(VkPipelineLayoutCreateInfo));
    layout_info.sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    layout_info.pushConstantRangeCount = 0;
    layout_info.pPushConstantRanges    = NULL;
    layout_info.setLayoutCount         = 2;
    layout_info.pSetLayouts            = setLayouts;

    if (vkCreatePipelineLayout(context->device, &layout_info, &context->allocator, &shader->layout) != VK_SUCCESS) {
        nu_error(NUVK_LOGGER_NAME"Failed to create postprocess pipeline layout.\n");
        return NU_FAILURE;
    }

    /* set stage info */
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
    vkDestroyPipelineLayout(context->device, shader->layout, &context->allocator);
    vkDestroyShaderModule(context->device, shader->vertex, &context->allocator);
    vkDestroyShaderModule(context->device, shader->fragment, &context->allocator);

    return NU_SUCCESS;
}