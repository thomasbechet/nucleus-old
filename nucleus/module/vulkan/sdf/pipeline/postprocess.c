#include <nucleus/module/vulkan/sdf/pipeline/postprocess.h>

#include <nucleus/module/vulkan/sdf/pipeline/sources.h>

static nu_result_t create_modules(
    nuvk_sdf_pipeline_postprocess_t *pipeline,
    const nuvk_context_t *context,
    const nuvk_shader_manager_t *shader_manager,
    const nu_string_t *sources
)
{
    nu_result_t result;

    /* vertex shader */
    result = nuvk_shader_module_create_from_glsl_source(context, shader_manager, VK_SHADER_STAGE_VERTEX_BIT,
        sources[NUVK_SDF_PIPELINE_SOURCE_POSTPROCESS_VERT], "postprocess.vert", &pipeline->vertex);
    if (result != NU_SUCCESS) {
        nu_error(NUVK_LOGGER_NAME"Failed to create postprocess vertex shader.\n");
        return result;
    }

    /* fragment shader */
    nu_string_t fragment_source;
    nu_string_allocate_copy(sources[NUVK_SDF_PIPELINE_SOURCE_POSTPROCESS_FRAG], &fragment_source);
    nu_string_replace(&fragment_source, NUVK_SDF_PIPELINE_INJECT_CONSTANTS, sources[NUVK_SDF_PIPELINE_SOURCE_CONSTANTS_GLSL]);
    result = nuvk_shader_module_create_from_glsl_source(context, shader_manager, VK_SHADER_STAGE_FRAGMENT_BIT, 
        fragment_source, "postprocess.frag", &pipeline->fragment);
    nu_string_free(fragment_source);
    if (result != NU_SUCCESS) {
        nu_error(NUVK_LOGGER_NAME"Failed to create postprocess fragment shader.\n");
        return result;
    }

    return NU_SUCCESS;
}

static nu_result_t create_layout(
    nuvk_sdf_pipeline_postprocess_t *pipeline,
    const nuvk_context_t *context,
    const nuvk_sdf_descriptors_t *descriptors
)
{
    VkDescriptorSetLayout set_layouts[2] = {
        descriptors->low_frequency.layout,
        descriptors->postprocess.layout
    };

    VkPipelineLayoutCreateInfo layout_info;
    memset(&layout_info, 0, sizeof(VkPipelineLayoutCreateInfo));
    layout_info.sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    layout_info.pushConstantRangeCount = 0;
    layout_info.pPushConstantRanges    = NULL;
    layout_info.setLayoutCount         = 2;
    layout_info.pSetLayouts            = set_layouts;

    if (vkCreatePipelineLayout(context->device, &layout_info, &context->allocator, &pipeline->layout) != VK_SUCCESS) {
        nu_error(NUVK_LOGGER_NAME"Failed to create postprocess pipeline layout.\n");
        return NU_FAILURE;
    }

    return NU_SUCCESS;
}

static nu_result_t create_pipeline(
    nuvk_sdf_pipeline_postprocess_t *pipeline,
    const nuvk_context_t *context,
    VkRenderPass renderpass
)
{
    VkPipelineVertexInputStateCreateInfo vertex_info_state;
    memset(&vertex_info_state, 0, sizeof(VkPipelineVertexInputStateCreateInfo));
    vertex_info_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

    VkPipelineInputAssemblyStateCreateInfo input_assembly_state;
    memset(&input_assembly_state, 0, sizeof(VkPipelineInputAssemblyStateCreateInfo));
    input_assembly_state.sType    = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    input_assembly_state.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

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
    color_blend_state.pAttachments    = &color_blend_attachment_state;

    VkDynamicState dynamic_states[] = {
        VK_DYNAMIC_STATE_VIEWPORT, 
        VK_DYNAMIC_STATE_SCISSOR
    };

    VkPipelineDynamicStateCreateInfo dynamic_state;
    memset(&dynamic_state, 0, sizeof(VkPipelineDynamicStateCreateInfo));
    dynamic_state.sType             = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamic_state.pDynamicStates    = dynamic_states; 
    dynamic_state.dynamicStateCount = 2;

    VkPipelineViewportStateCreateInfo viewport_state;
    memset(&viewport_state, 0, sizeof(VkPipelineViewportStateCreateInfo));
    viewport_state.sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewport_state.viewportCount = 1;
    viewport_state.scissorCount  = 1;

    VkPipelineShaderStageCreateInfo stages[2];
    memset(stages, 0, sizeof(VkPipelineShaderStageCreateInfo) * 2);
    stages[0].sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    stages[0].stage  = VK_SHADER_STAGE_VERTEX_BIT;
    stages[0].pName  = "main";
    stages[0].module = pipeline->vertex;
    stages[1].sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    stages[1].stage  = VK_SHADER_STAGE_FRAGMENT_BIT;
    stages[1].pName  = "main";
    stages[1].module = pipeline->fragment;

    VkGraphicsPipelineCreateInfo pipeline_info;
    memset(&pipeline_info, 0, sizeof(VkGraphicsPipelineCreateInfo));
    pipeline_info.sType               = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipeline_info.stageCount          = 2;
    pipeline_info.pStages             = stages;
    pipeline_info.pVertexInputState   = &vertex_info_state;
    pipeline_info.pInputAssemblyState = &input_assembly_state;
    pipeline_info.pViewportState      = &viewport_state;
    pipeline_info.pRasterizationState = &rasterization_state;
    pipeline_info.pMultisampleState   = &multisample_state;
    pipeline_info.pColorBlendState    = &color_blend_state;
    pipeline_info.layout              = pipeline->layout;
    pipeline_info.renderPass          = renderpass;
    pipeline_info.subpass             = 0;
    pipeline_info.pDynamicState       = &dynamic_state;
    pipeline_info.basePipelineHandle  = VK_NULL_HANDLE;

    if (vkCreateGraphicsPipelines(context->device, VK_NULL_HANDLE, 1, &pipeline_info, &context->allocator, &pipeline->pipeline) != VK_SUCCESS) {
        nu_error(NUVK_LOGGER_NAME"Failed to create postprocess pipeline.\n");
        return NU_FAILURE;
    }

    return NU_SUCCESS;
}

nu_result_t nuvk_sdf_pipeline_postprocess_create(
    nuvk_sdf_pipeline_postprocess_t *pipeline,
    const nuvk_context_t *context,
    const nuvk_shader_manager_t *shader_manager,
    const nuvk_sdf_descriptors_t *descriptors,
    VkRenderPass postprocess_renderpass,
    const nu_string_t *sources
)
{
    nu_result_t result = NU_SUCCESS;

    result &= create_modules(pipeline, context, shader_manager, sources);
    result &= create_layout(pipeline, context, descriptors);
    result &= create_pipeline(pipeline, context, postprocess_renderpass);

    return result;
}
nu_result_t nuvk_sdf_pipeline_postprocess_destroy(
    nuvk_sdf_pipeline_postprocess_t *pipeline,
    const nuvk_context_t *context
)
{
    vkDestroyPipeline(context->device, pipeline->pipeline, &context->allocator);
    vkDestroyPipelineLayout(context->device, pipeline->layout, &context->allocator);
    vkDestroyShaderModule(context->device, pipeline->vertex, &context->allocator);
    vkDestroyShaderModule(context->device, pipeline->fragment, &context->allocator);

    return NU_SUCCESS;
}