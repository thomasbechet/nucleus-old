#include "graphics_pipeline.h"

#include "../common/logger.h"
#include "../context/context.h"
#include "shader.h"
#include "render_pass.h"

typedef struct {
    VkShaderModule vert_shader_module;
    VkShaderModule frag_shader_module;
    VkPipelineLayout pipeline_layout;
    VkPipeline pipeline;
} nuvk_graphics_pipeline_data_t;

static nuvk_graphics_pipeline_data_t _data;

nu_result_t nuvk_graphics_pipeline_create(void)
{
    const nuvk_context_t *ctx = nuvk_context_get();

    nu_result_t result;
    result = NU_SUCCESS;

    /* create shader modules */
    result = nuvk_shader_module_create(&_data.vert_shader_module, "engine/shaders/shader.vert.spv");
    if (result != NU_SUCCESS) return NU_FAILURE;
    result = nuvk_shader_module_create(&_data.frag_shader_module, "engine/shaders/shader.frag.spv");
    if (result != NU_SUCCESS) return NU_FAILURE;

    /* create shader stage */
    VkPipelineShaderStageCreateInfo vert_shader_stage_info;
    memset(&vert_shader_stage_info, 0, sizeof(VkPipelineShaderStageCreateInfo));
    vert_shader_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vert_shader_stage_info.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vert_shader_stage_info.module = _data.vert_shader_module;
    vert_shader_stage_info.pName = "main";

    VkPipelineShaderStageCreateInfo frag_shader_stage_info;
    memset(&frag_shader_stage_info, 0, sizeof(VkPipelineShaderStageCreateInfo));
    frag_shader_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    frag_shader_stage_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    frag_shader_stage_info.module = _data.frag_shader_module;
    frag_shader_stage_info.pName = "main";

    VkPipelineShaderStageCreateInfo shader_stages[] = {vert_shader_stage_info, frag_shader_stage_info};

    /* create vertex input */
    VkPipelineVertexInputStateCreateInfo vertex_input_info;
    memset(&vertex_input_info, 0, sizeof(VkPipelineVertexInputStateCreateInfo));
    vertex_input_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertex_input_info.vertexBindingDescriptionCount = 0;
    vertex_input_info.pVertexBindingDescriptions = NULL;
    vertex_input_info.vertexAttributeDescriptionCount = 0;
    vertex_input_info.pVertexAttributeDescriptions = NULL;

    /* create input assembly */
    VkPipelineInputAssemblyStateCreateInfo input_assembly;
    memset(&input_assembly, 0, sizeof(VkPipelineInputAssemblyStateCreateInfo));
    input_assembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    input_assembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    input_assembly.primitiveRestartEnable = VK_FALSE;

    /* viewports and scissors */
    VkViewport viewport;
    VkRect2D scissor;
    VkExtent2D swapchain_extent;
    memset(&viewport, 0, sizeof(VkViewport));
    memset(&scissor, 0, sizeof(VkRect2D));
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    swapchain_extent = ctx->swapchain.extent;
    viewport.width = (float)swapchain_extent.width;
    viewport.height = (float)swapchain_extent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    scissor.offset.x = 0.0f;
    scissor.offset.y = 0.0f;
    scissor.extent = swapchain_extent;

    /* create viewport state */
    VkPipelineViewportStateCreateInfo viewport_state;
    memset(&viewport_state, 0, sizeof(VkPipelineViewportStateCreateInfo));
    viewport_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewport_state.viewportCount = 1;
    viewport_state.pViewports = &viewport;
    viewport_state.scissorCount = 1;
    viewport_state.pScissors = &scissor;

    /* rasterizer */
    VkPipelineRasterizationStateCreateInfo rasterizer;
    memset(&rasterizer, 0, sizeof(VkPipelineRasterizationStateCreateInfo));
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;

    /* multisampling */
    VkPipelineMultisampleStateCreateInfo multisampling;
    memset(&multisampling, 0, sizeof(VkPipelineMultisampleStateCreateInfo));
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisampling.minSampleShading = 1.0f;
    multisampling.pSampleMask = NULL;
    multisampling.alphaToCoverageEnable = VK_FALSE;
    multisampling.alphaToOneEnable = VK_FALSE;

    /* color blending */
    VkPipelineColorBlendAttachmentState color_blend_attachment;
    memset(&color_blend_attachment, 0, sizeof(VkPipelineColorBlendAttachmentState));
    color_blend_attachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    color_blend_attachment.blendEnable = VK_FALSE;
    color_blend_attachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    color_blend_attachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    color_blend_attachment.colorBlendOp = VK_BLEND_OP_ADD;
    color_blend_attachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    color_blend_attachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    color_blend_attachment.alphaBlendOp = VK_BLEND_OP_ADD;

    VkPipelineColorBlendStateCreateInfo color_blending;
    memset(&color_blending, 0, sizeof(VkPipelineColorBlendStateCreateInfo));
    color_blending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    color_blending.logicOpEnable = VK_FALSE;
    color_blending.attachmentCount = 1;
    color_blending.pAttachments = &color_blend_attachment;

    /* dynamic state */
    VkDynamicState dynamic_states[] = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_LINE_WIDTH
    };

    VkPipelineDynamicStateCreateInfo dynamic_state;
    memset(&dynamic_state, 0, sizeof(VkPipelineDynamicStateCreateInfo));
    dynamic_state.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamic_state.dynamicStateCount = 2;
    dynamic_state.pDynamicStates = dynamic_states;

    /* pipeline layout */
    VkPipelineLayoutCreateInfo pipeline_layout_info;
    memset(&pipeline_layout_info, 0, sizeof(VkPipelineLayoutCreateInfo));
    pipeline_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    
    if (vkCreatePipelineLayout(ctx->device, &pipeline_layout_info, NULL, &_data.pipeline_layout) != VK_SUCCESS) {
        nu_warning(NUVK_VULKAN_LOG_NAME"Failed to create pipeline layout.\n");
        return NU_FAILURE;
    }

    /* create graphics pipeline */
    VkGraphicsPipelineCreateInfo pipeline_info;
    memset(&pipeline_info, 0, sizeof(VkGraphicsPipelineCreateInfo));
    pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;

    pipeline_info.stageCount = 2;
    pipeline_info.pStages = shader_stages;

    pipeline_info.pVertexInputState = &vertex_input_info;
    pipeline_info.pInputAssemblyState = &input_assembly;
    pipeline_info.pViewportState = &viewport_state;
    pipeline_info.pRasterizationState = &rasterizer;
    pipeline_info.pMultisampleState = &multisampling;
    pipeline_info.pDepthStencilState = NULL;
    pipeline_info.pColorBlendState = &color_blending;
    pipeline_info.pDynamicState = NULL;

    pipeline_info.layout = _data.pipeline_layout;

    pipeline_info.renderPass = nuvk_render_pass_get_handle();
    pipeline_info.subpass = 0;

    pipeline_info.basePipelineHandle = VK_NULL_HANDLE;
    pipeline_info.basePipelineIndex = -1;

    if (vkCreateGraphicsPipelines(ctx->device, VK_NULL_HANDLE, 1, &pipeline_info, NULL, &_data.pipeline) != VK_SUCCESS) {
        nu_warning(NUVK_VULKAN_LOG_NAME"Failed to create graphics pipeline.\n");
        return NU_SUCCESS;
    }

    return result;
}
nu_result_t nuvk_graphics_pipeline_destroy(void)
{
    const nuvk_context_t *ctx = nuvk_context_get();

    /* destroy pipeline */
    vkDestroyPipeline(ctx->device, _data.pipeline, NULL);

    /* destroy pipeline layout */
    vkDestroyPipelineLayout(ctx->device, _data.pipeline_layout, NULL);

    /* destroy shader modules */
    nuvk_shader_module_destroy(_data.vert_shader_module);
    nuvk_shader_module_destroy(_data.frag_shader_module);

    return NU_SUCCESS;
}

VkPipeline nuvk_graphics_pipeline_get_handle(void)
{
    return _data.pipeline;
}