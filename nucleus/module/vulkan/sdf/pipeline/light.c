#include <nucleus/module/vulkan/sdf/pipeline/light.h>

#include <nucleus/module/vulkan/sdf/pipeline/sources.h>

static nu_result_t create_modules(
    nuvk_sdf_pipeline_light_t *pipeline,
    const nuvk_context_t *context,
    const nuvk_shader_manager_t *shader_manager,
    const nu_string_t *sources
)
{
    nu_result_t result;

    nu_string_t compute_source;
    nu_string_allocate_copy(sources[NUVK_SDF_PIPELINE_SOURCE_LIGHT_COMP], &compute_source);
    nu_string_replace(&compute_source, NUVK_SDF_PIPELINE_INJECT_CONSTANTS, sources[NUVK_SDF_PIPELINE_SOURCE_CONSTANTS_GLSL]);

    result = nuvk_shader_module_create_from_glsl_source(context, shader_manager, VK_SHADER_STAGE_COMPUTE_BIT,
        compute_source, "light.comp", &pipeline->compute);
    nu_string_free(compute_source);
    if (result != NU_SUCCESS) {
        nu_error(NUVK_LOGGER_NAME"Failed to create light compute shader.\n");
        return NU_FAILURE;
    }

    return NU_SUCCESS;
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

    if (vkCreatePipelineLayout(context->device, &layout_info, &context->allocator, &pipeline->layout) != VK_SUCCESS) {
        nu_error(NUVK_LOGGER_NAME"Failed to create light pipeline layout.\n");
        return NU_FAILURE;
    }

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

    if (vkCreateComputePipelines(context->device, VK_NULL_HANDLE, 1, &info, &context->allocator, &pipeline->pipeline) != VK_SUCCESS) {
        nu_error(NUVK_LOGGER_NAME"Failed to create light pipeline.\n");
        return NU_FAILURE;
    }

    return NU_SUCCESS;
}

nu_result_t nuvk_sdf_pipeline_light_create(
    nuvk_sdf_pipeline_light_t *pipeline,
    const nuvk_context_t *context,
    const nuvk_shader_manager_t *shader_manager,
    const nuvk_sdf_descriptors_t *descriptors,
    const nu_string_t *sources
)
{
    nu_result_t result = NU_SUCCESS;
    
    result &= create_modules(pipeline, context, shader_manager, sources);
    result &= create_layout(pipeline, context, descriptors);
    result &= create_pipeline(pipeline, context);

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