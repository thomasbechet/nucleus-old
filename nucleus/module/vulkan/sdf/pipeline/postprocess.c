#include <nucleus/module/vulkan/sdf/pipeline/postprocess.h>

nu_result_t nuvk_sdf_pipeline_postprocess_create(
    nuvk_sdf_pipeline_postprocess_t *pipeline,
    const nuvk_context_t *context,
    const nuvk_swapchain_t *swapchain,
    const nuvk_sdf_shader_postprocess_t *shader,
    const nuvk_sdf_descriptors_t *descriptors,
    VkRenderPass postprocess_renderpass
)
{
    /* create layout */
    VkPipelineLayoutCreateInfo layout_info;
    memset(&layout_info, 0, sizeof(VkPipelineLayoutCreateInfo));
    layout_info.sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    layout_info.pushConstantRangeCount = 0;
    layout_info.pPushConstantRanges    = NULL;
    layout_info.setLayoutCount         = 1;
    layout_info.pSetLayouts            = &descriptors->low_frequency.layout;

    if (vkCreatePipelineLayout(context->device, &layout_info, &context->allocator, &pipeline->layout) != VK_SUCCESS) {
        nu_error(NUVK_LOGGER_NAME"Failed to create postprocess pipeline layout.\n");
        return NU_FAILURE;
    }

    /* create pipeline */
    VkPipelineVertexInputStateCreateInfo vertex_info_state;
    memset(&vertex_info_state, 0, sizeof(VkPipelineVertexInputStateCreateInfo));
    vertex_info_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

    VkPipelineInputAssemblyStateCreateInfo input_assembly_state;
    memset(&input_assembly_state, 0, sizeof(VkPipelineInputAssemblyStateCreateInfo));
    input_assembly_state.sType    = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    input_assembly_state.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

    VkViewport viewport;
    viewport.x        = 0;
    viewport.y        = swapchain->extent.height;
    viewport.width    = swapchain->extent.width;
    viewport.height   = -(int32_t)swapchain->extent.height;
    viewport.minDepth = 0.0;
    viewport.maxDepth = 1.0;

    VkRect2D scissor;
    scissor.offset.x = 0;
    scissor.offset.y = 0;
    scissor.extent   = swapchain->extent;

    VkPipelineViewportStateCreateInfo viewport_state;
    memset(&viewport_state, 0, sizeof(VkPipelineViewportStateCreateInfo));
    viewport_state.sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewport_state.viewportCount = 1;
    viewport_state.pViewports    = &viewport;
    viewport_state.scissorCount  = 1;
    viewport_state.pScissors     = &scissor;

    VkPipelineRasterizationStateCreateInfo rasterization_state;
    memset(&rasterization_state, 0, sizeof(VkPipelineRasterizationStateCreateInfo));
    rasterization_state.sType       = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterization_state.polygonMode = VK_POLYGON_MODE_FILL;
    rasterization_state.cullMode    = VK_CULL_MODE_NONE;
    rasterization_state.frontFace   = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterization_state.lineWidth   = 1.0f;

    VkPipelineMultisampleStateCreateInfo multisample_state;
    memset(&multisample_state, 0, sizeof(VkPipelineMultisampleStateCreateInfo));
    multisample_state.sType                = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisample_state.sampleShadingEnable  = VK_FALSE;
    multisample_state.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    VkPipelineColorBlendAttachmentState color_blend_attachment_state;
    memset(&color_blend_attachment_state, 0, sizeof(VkPipelineColorBlendAttachmentState));
    color_blend_attachment_state.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    color_blend_attachment_state.blendEnable    = VK_FALSE;

    VkPipelineColorBlendStateCreateInfo color_blend_state;
    memset(&color_blend_state, 0, sizeof(VkPipelineColorBlendStateCreateInfo));
    color_blend_state.sType           = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    color_blend_state.logicOpEnable   = VK_FALSE;
    color_blend_state.logicOp         = VK_LOGIC_OP_COPY;
    color_blend_state.attachmentCount = 1;
    color_blend_state.pAttachments    = &color_blend_attachment_state;;

    VkGraphicsPipelineCreateInfo pipeline_info;
    memset(&pipeline_info, 0, sizeof(VkGraphicsPipelineCreateInfo));
    pipeline_info.sType               = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipeline_info.stageCount          = shader->stage_count;
    pipeline_info.pStages             = shader->stages;
    pipeline_info.pVertexInputState   = &vertex_info_state;
    pipeline_info.pInputAssemblyState = &input_assembly_state;
    pipeline_info.pViewportState      = &viewport_state;
    pipeline_info.pRasterizationState = &rasterization_state;
    pipeline_info.pMultisampleState   = &multisample_state;
    pipeline_info.pColorBlendState    = &color_blend_state;
    pipeline_info.layout              = pipeline->layout;
    pipeline_info.renderPass          = postprocess_renderpass;
    pipeline_info.subpass             = 0;
    pipeline_info.pDynamicState       = NULL;
    pipeline_info.basePipelineHandle  = VK_NULL_HANDLE;

    if (vkCreateGraphicsPipelines(context->device, VK_NULL_HANDLE, 1, &pipeline_info, &context->allocator, &pipeline->pipeline) != VK_SUCCESS) {
        nu_error(NUVK_LOGGER_NAME"Failed to create postprocess pipeline.\n");
        return NU_FAILURE;
    }

    return NU_SUCCESS;
}
nu_result_t nuvk_sdf_pipeline_postprocess_destroy(
    nuvk_sdf_pipeline_postprocess_t *pipeline,
    const nuvk_context_t *context
)
{
    vkDestroyPipeline(context->device, pipeline->pipeline, &context->allocator);
    vkDestroyPipelineLayout(context->device, pipeline->layout, &context->allocator);

    return NU_SUCCESS;
}