#include <nucleus/module/vulkan/core/pipeline.h>

nu_result_t nuvk_pipeline_create_graphics(
    nuvk_pipeline_t *pipeline,
    const nuvk_context_t *context,
    const VkPipelineLayoutCreateInfo *layout_info,
    const VkGraphicsPipelineCreateInfo *pipeline_info
)
{
    if (vkCreatePipelineLayout(context->device, layout_info, &context->allocator, &pipeline->layout) != VK_SUCCESS) {
        return NU_FAILURE;
    }
    if (vkCreateGraphicsPipelines(context->device, VK_NULL_HANDLE, 1, pipeline_info, &context->allocator, &pipeline->pipeline) != VK_SUCCESS) {
        return NU_FAILURE;
    }

    return NU_SUCCESS;
}
nu_result_t nuvk_pipeline_destroy(
    nuvk_pipeline_t *pipeline,
    const nuvk_context_t *context
)
{
    vkDestroyPipeline(context->device, pipeline->pipeline, &context->allocator);
    vkDestroyPipelineLayout(context->device, pipeline->layout, &context->allocator);

    return NU_SUCCESS;
}