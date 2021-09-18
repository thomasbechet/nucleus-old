#include <nucleus/module/vulkan/sdf/pipeline/geometry.h>

#include <nucleus/module/vulkan/sdf/pipeline/sources.h>

static void generate_global_geometry_glsl_source(
    const nuvk_sdf_instance_type_info_t *types,
    uint32_t type_count,
    nu_string_t *source
)
{
    /* clear source */
    nu_string_clear(source);

    /* constants */
    if (type_count > 0) {
        nu_string_append_cstr(source, "#define NO_INSTANCE_TYPES true\n");
    }

    /* append instance definition */
    for (uint32_t i = 0; i < type_count; i++) {
        nu_string_t instance_definition;
        nu_string_allocate_format(&instance_definition,
            "struct InstanceData%ld {\n"
            "%s\n"
            "};\n"
            "struct Instance%ld {\n"
            "	mat3 invRotation;\n"
            "	vec3 translation;\n"
            "	vec3 scale;\n"
            "	InstanceData%ld data;\n"
            "};\n"
            "float sdf%ld(in vec3 p, in InstanceData%ld data) {\n"
            "   %s\n"
            "}\n",
            i, types[i].glsl_data_code, i, i, i, i, types[i].glsl_sdf_code);
        nu_string_append(source, instance_definition);
        nu_string_free(instance_definition);
    }

    /* append index ubo */
    nu_string_append_cstr(source, "layout(set = 0, binding = 1, std140) uniform IndexUBO {\n");
    for (uint32_t i = 0; i < type_count; i++) {
        nu_string_t instance_block;
        nu_string_allocate_format(&instance_block,
            "   uint indexCount%ld;\n"
            "   uint indices%ld[%ld];\n",
            i, i, types[i].max_instance_count);
        nu_string_append(source, instance_block);
        nu_free(instance_block);
    }
    if (type_count == 0) {
        nu_string_append_cstr(source, "  uint unusedIndex;\n");
    }
    nu_string_append_cstr(source, "};\n");

    /* append instance ubo */
    nu_string_append_cstr(source, "layout(set = 0, binding = 2, std140) uniform InstanceUBO {\n");
    for (uint32_t i = 0; i < type_count; i++) {
        nu_string_t instance_line;
        nu_string_allocate_format(&instance_line,
            "   Instance%ld instances%ld[%ld];\n",
            i, i, types[i].max_instance_count);
        nu_string_append(source, instance_line);
        nu_string_free(instance_line);
    }
    if (type_count == 0) {
        nu_string_append_cstr(source, "  uint unusedInstance;\n");
    }
    nu_string_append_cstr(source, "};\n");

    nu_string_append_cstr(source, 
        "float tracePrimary(in vec3 pos, in vec3 dir, out vec3 normal) {\n"
        "   float hitDepth = MAX_DISTANCE;\n");
    for (uint32_t i = 0; i < type_count; i++) {
        nu_string_t instance_code;
        nu_string_allocate_format(&instance_code,
            "   for (uint i = 0; i < indexCount%ld; i++) {\n"
            "       uint index = indices%ld[i];\n"
            "       vec3 relPos = instances%ld[index].invRotation * (pos - instances%ld[index].translation);\n"
            "       vec3 relDir = normalize(instances%ld[index].invRotation * dir);\n"
            "       InstanceData%ld data = instances%ld[index].data;\n"
            "       float depth = 0.0;\n"
            "       for (int step = 0; depth < hitDepth && step < MAX_RAYMARCH_STEP; step++) {\n"
            "           vec3 p = relPos + depth * relDir;\n"
            "           float sd = sdf%ld(p, data);\n"
            "           if (sd < MIN_HIT_DISTANCE) {\n"
            "               const vec2 e = vec2(EPSILON, 0);\n"
            "               normal = normalize(vec3(\n"
            "                   sd - sdf%ld(vec3(p - e.xyy), data),\n"
            "                   sd - sdf%ld(vec3(p - e.yxy), data),\n"
            "                   sd - sdf%ld(vec3(p - e.yyx), data)\n"
            "               ));\n"
            "               normal = transpose(instances%ld[index].invRotation) * normal;\n"
            "               hitDepth = depth;\n"
            "               break;\n"
            "           }\n"
            "           depth += sd;\n"
            "       }\n"
            "    }\n",
            i, i, i, i, i, i, i, i, i, i, i, i);
        nu_string_append(source, instance_code);
        nu_string_free(instance_code);
    }
    nu_string_append_cstr(source,
        "   return hitDepth;\n"
        "}\n");
}

static nu_result_t create_modules(
    nuvk_sdf_pipeline_geometry_t *pipeline,
    const nuvk_context_t *context,
    const nuvk_shader_manager_t *shader_manager,
    const nu_string_t *sources,
    const nuvk_sdf_instance_type_info_t *types,
    uint32_t type_count
)
{
    nu_result_t result;

    /* vertex module */
    result = nuvk_shader_module_create_from_glsl_source(context, shader_manager, VK_SHADER_STAGE_VERTEX_BIT,
        sources[NUVK_SDF_PIPELINE_SOURCE_GEOMETRY_TEMPLATE_VERT], "geometry.vert", &pipeline->vertex);
    NU_CHECK(result == NU_SUCCESS, return result, NUVK_LOGGER_NAME, "Failed to create geometry vertex shader.");

    /* create fragment module with injected code */
    nu_string_t inject_instances_source;
    nu_string_t fragment_source;
    nu_string_allocate(&inject_instances_source);
    generate_global_geometry_glsl_source(types, type_count, &inject_instances_source);
    nu_string_allocate_copy(&fragment_source, sources[NUVK_SDF_PIPELINE_SOURCE_GEOMETRY_TEMPLATE_FRAG]);
    nu_string_replace(&fragment_source, NUVK_SDF_PIPELINE_INJECT_CONSTANTS, sources[NUVK_SDF_PIPELINE_SOURCE_CONSTANTS_GLSL]);
    nu_string_replace(&fragment_source, NUVK_SDF_PIPELINE_INJECT_INSTANCES, inject_instances_source);
    
    /* fragment module */
    result = nuvk_shader_module_create_from_glsl_source(context, shader_manager,
        VK_SHADER_STAGE_FRAGMENT_BIT, fragment_source, "geometry.frag", &pipeline->fragment);
    nu_string_free(inject_instances_source);
    nu_string_free(fragment_source);
    NU_CHECK(result == NU_SUCCESS, return result, NUVK_LOGGER_NAME, "Failed to create geometry pipeline layout.");

    return NU_SUCCESS;
}

static nu_result_t create_layout(
    nuvk_sdf_pipeline_geometry_t *pipeline,
    const nuvk_context_t *context,
    const nuvk_sdf_descriptors_t *descriptors
)
{
    VkPipelineLayoutCreateInfo layout_info;
    memset(&layout_info, 0, sizeof(VkPipelineLayoutCreateInfo));
    layout_info.sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    layout_info.pushConstantRangeCount = 0;
    layout_info.pPushConstantRanges    = NULL;
    layout_info.setLayoutCount         = 1;
    layout_info.pSetLayouts            = &descriptors->low_frequency.layout;

    VkResult result = vkCreatePipelineLayout(context->device, &layout_info, &context->allocator, &pipeline->layout);
    NU_CHECK(result == VK_SUCCESS, return NU_FAILURE, NUVK_LOGGER_NAME, "Failed to create geometry pipeline layout.");

    return NU_SUCCESS;
}

static nu_result_t create_pipeline(
    nuvk_sdf_pipeline_geometry_t *pipeline,
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

    VkResult result = vkCreateGraphicsPipelines(context->device, VK_NULL_HANDLE, 1, &pipeline_info, &context->allocator, &pipeline->pipeline);
    NU_CHECK(result == VK_SUCCESS, return NU_FAILURE, NUVK_LOGGER_NAME, "Failed to create geometry pipeline.");

    return NU_SUCCESS;
}

nu_result_t nuvk_sdf_pipeline_geometry_create(
    nuvk_sdf_pipeline_geometry_t *pipeline,
    const nuvk_context_t *context,
    const nuvk_shader_manager_t *shader_manager,
    const nuvk_sdf_descriptors_t *descriptors,
    VkRenderPass geometry_renderpass,
    const nu_string_t *sources
)
{
    nu_result_t result;

    result = create_modules(pipeline, context, shader_manager, sources, NULL, 0);
    NU_CHECK(result == NU_SUCCESS, return result, NUVK_LOGGER_NAME, "Failed to create modules.");
    result = create_layout(pipeline, context, descriptors);
    NU_CHECK(result == NU_SUCCESS, return result, NUVK_LOGGER_NAME, "Failed to create layout.");
    result = create_pipeline(pipeline, context, geometry_renderpass);
    NU_CHECK(result == NU_SUCCESS, return result, NUVK_LOGGER_NAME, "Failed to create pipeline.");

    return NU_SUCCESS;
}
nu_result_t nuvk_sdf_pipeline_geometry_destroy(
    nuvk_sdf_pipeline_geometry_t *pipeline,
    const nuvk_context_t *context
)
{
    vkDestroyPipeline(context->device, pipeline->pipeline, &context->allocator);
    vkDestroyPipelineLayout(context->device, pipeline->layout, &context->allocator);
    vkDestroyShaderModule(context->device, pipeline->vertex, &context->allocator);
    vkDestroyShaderModule(context->device, pipeline->fragment, &context->allocator);

    return NU_SUCCESS;
}
nu_result_t nuvk_sdf_pipeline_geometry_recompile(
    nuvk_sdf_pipeline_geometry_t *pipeline,
    const nuvk_context_t *context,
    const nuvk_shader_manager_t *shader_manager,
    VkRenderPass geometry_renderpass,
    const nu_string_t *sources,
    const nuvk_sdf_instance_type_info_t *types,
    uint32_t type_count
)
{
    /* destroy resources */
    vkDestroyPipeline(context->device, pipeline->pipeline, &context->allocator);
    vkDestroyShaderModule(context->device, pipeline->vertex, &context->allocator);
    vkDestroyShaderModule(context->device, pipeline->fragment, &context->allocator);

    /* recreate modules and shaders */
    nu_result_t result;
    
    result = create_modules(pipeline, context, shader_manager, sources, types, type_count);
    NU_CHECK(result == NU_SUCCESS, return result, NUVK_LOGGER_NAME, "Failed to create modules.");
    result = create_pipeline(pipeline, context, geometry_renderpass);
    NU_CHECK(result == NU_SUCCESS, return result, NUVK_LOGGER_NAME, "Failed to create pipeline.");

    return result;
}