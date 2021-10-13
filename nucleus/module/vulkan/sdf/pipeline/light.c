#include <nucleus/module/vulkan/sdf/pipeline/light.h>

static nu_result_t create_modules(
    nuvk_sdf_pipeline_light_t *pipeline,
    const nuvk_context_t *context,
    const nuvk_shader_manager_t *shader_manager,
    const nuvk_sdf_pipeline_generator_t *generator
)
{
    nu_result_t result = NU_SUCCESS;

    /* generate source code */
    nu_string_t source;
    nu_string_allocate(&source);
    nuvk_sdf_pipeline_generator_get_source(generator, NUVK_SDF_PIPELINE_SOURCE_LIGHT_COMP, &source);

    /* compile */
    result = nuvk_shader_module_create_from_glsl_source(context, shader_manager, 
        VK_SHADER_STAGE_COMPUTE_BIT, source, "light.comp", &pipeline->compute);
    NU_CHECK(result == NU_SUCCESS, goto cleanup0, NUVK_LOGGER_NAME, "Failed to create light compute shader.");
    
cleanup0:
    nu_string_free(source);
    
    return result;
}

static nu_result_t create_layout(
    nuvk_sdf_pipeline_light_t *pipeline,
    const nuvk_context_t *context,
    const nuvk_sdf_descriptors_t *descriptors
)
{
    VkDescriptorSetLayout set_layouts[2] = {
        descriptors->low_frequency.layout,
        descriptors->light.layout
    };

    VkPipelineLayoutCreateInfo layout_info;
    memset(&layout_info, 0, sizeof(VkPipelineLayoutCreateInfo));
    layout_info.sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    layout_info.pushConstantRangeCount = 0;
    layout_info.pPushConstantRanges    = NULL;
    layout_info.setLayoutCount         = 2;
    layout_info.pSetLayouts            = set_layouts;

    VkResult result = vkCreatePipelineLayout(context->device, &layout_info, &context->allocator, &pipeline->layout);
    NU_CHECK(result == VK_SUCCESS, return result, NUVK_LOGGER_NAME, "Failed to create light pipeline layout.");

    return NU_SUCCESS;
}

static nu_result_t create_pipeline(
    nuvk_sdf_pipeline_light_t *pipeline,
    const nuvk_context_t *context
)
{
    VkComputePipelineCreateInfo info;
    memset(&info, 0, sizeof(VkComputePipelineCreateInfo));

    info.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
    info.layout       = pipeline->layout;
    info.stage.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    info.stage.stage  = VK_SHADER_STAGE_COMPUTE_BIT;
    info.stage.pName  = "main";
    info.stage.module = pipeline->compute;

    VkResult result = vkCreateComputePipelines(context->device, VK_NULL_HANDLE, 1, &info, &context->allocator, &pipeline->pipeline);
    NU_CHECK(result == VK_SUCCESS, return result, NUVK_LOGGER_NAME, "Failed to create light pipeline.");

    return NU_SUCCESS;
}

nu_result_t nuvk_sdf_pipeline_light_create(
    nuvk_sdf_pipeline_light_t *pipeline,
    const nuvk_context_t *context,
    const nuvk_shader_manager_t *shader_manager,
    const nuvk_sdf_descriptors_t *descriptors,
    const nuvk_sdf_pipeline_generator_t *generator
)
{
    nu_result_t result;
    
    result = create_modules(pipeline, context, shader_manager, generator);
    NU_CHECK(result == NU_SUCCESS, return result, NUVK_LOGGER_NAME, "Failed to create modules.");
    result = create_layout(pipeline, context, descriptors);
    NU_CHECK(result == NU_SUCCESS, return result, NUVK_LOGGER_NAME, "Failed to create layout.");
    result = create_pipeline(pipeline, context);
    NU_CHECK(result == NU_SUCCESS, return result, NUVK_LOGGER_NAME, "Failed to create pipeline.");

    return result;
}
nu_result_t nuvk_sdf_pipeline_light_destroy(
    nuvk_sdf_pipeline_light_t *pipeline,
    const nuvk_context_t *context
)
{
    vkDestroyPipeline(context->device, pipeline->pipeline, &context->allocator);
    vkDestroyPipelineLayout(context->device, pipeline->layout, &context->allocator);
    vkDestroyShaderModule(context->device, pipeline->compute, &context->allocator);

    return NU_SUCCESS;
}
nu_result_t nuvk_sdf_pipeline_light_recompile(
    nuvk_sdf_pipeline_light_t *pipeline,
    const nuvk_context_t *context,
    const nuvk_shader_manager_t *shader_manager,
    const nuvk_sdf_descriptors_t *descriptors,
    const nuvk_sdf_pipeline_generator_t *generator
)
{
    /* destroy resources */
    vkDestroyPipeline(context->device, pipeline->pipeline, &context->allocator);
    vkDestroyPipelineLayout(context->device, pipeline->layout, &context->allocator);
    vkDestroyShaderModule(context->device, pipeline->compute, &context->allocator);

    /* recreate modules and shaders */
    nu_result_t result;
    
    result = create_modules(pipeline, context, shader_manager, generator);
    NU_CHECK(result == NU_SUCCESS, return result, NUVK_LOGGER_NAME, "Failed to create modules.");
    result = create_layout(pipeline, context, descriptors);
    NU_CHECK(result == NU_SUCCESS, return result, NUVK_LOGGER_NAME, "Failed to create layout.");
    result = create_pipeline(pipeline, context);
    NU_CHECK(result == NU_SUCCESS, return result, NUVK_LOGGER_NAME, "Failed to create pipeline.");

    return result;
}