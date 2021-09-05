#include <nucleus/module/vulkan/sdf/renderer.h>

#include <nucleus/module/vulkan/sdf/scene/primitives.h>

nu_result_t nuvk_sdf_renderer_initialize(
    nuvk_sdf_renderer_t *renderer,
    const nuvk_context_t *context,
    const nuvk_memory_manager_t *memory_manager,
    const nuvk_shader_manager_t *shader_manager,
    const nuvk_swapchain_t *swapchain,
    const nuvk_render_context_t *render_context
)
{
    if (nuvk_sdf_buffers_initialize(&renderer->buffers, context, memory_manager, render_context) != NU_SUCCESS) {
        nu_error(NUVK_LOGGER_NAME"Failed to create buffers.\n");
        return NU_FAILURE;
    }

    if (nuvk_sdf_images_initialize(&renderer->images, context, memory_manager, swapchain) != NU_SUCCESS) {
        nu_error(NUVK_LOGGER_NAME"Failed to create images.\n");
        return NU_FAILURE;
    }

    if (nuvk_sdf_renderpasses_initialize(&renderer->renderpasses, context, swapchain, &renderer->images) != NU_SUCCESS) {
        nu_error(NUVK_LOGGER_NAME"Failed to create renderpasses.\n");
        return NU_FAILURE;
    }

    if (nuvk_sdf_framebuffers_initialize(&renderer->framebuffers, context, swapchain, &renderer->images, &renderer->renderpasses) != NU_SUCCESS) {
        nu_error(NUVK_LOGGER_NAME"Failed to create framebuffers.\n");
        return NU_FAILURE;
    }

    if (nuvk_sdf_descriptors_initialize(&renderer->descriptors, context, &renderer->buffers, &renderer->images) != NU_SUCCESS) {
        nu_error(NUVK_LOGGER_NAME"Failed to create descriptors.\n");
        return NU_FAILURE;
    }

    if (nuvk_sdf_pipelines_initialize(&renderer->pipelines, context, shader_manager, &renderer->descriptors, &renderer->renderpasses) != NU_SUCCESS) {
        nu_error(NUVK_LOGGER_NAME"Failed to create pipelines.\n");
        return NU_FAILURE;
    }

    nuvk_sdf_scene_initialize(&renderer->scene);

    nuvk_sdf_primitives_t primitives;
    nuvk_sdf_primitives_register(&primitives);
    
    nuvk_sdf_instance_info_t info;
    info.type  = primitives.sphere;
    info.flags = 0x0;
    nu_transform_identity(&info.transform);
    nuvk_sdf_sphere_data_t data;
    data.radius = 1.0f;
    info.data = &data;
    nuvk_sdf_instance_t handle;
    
    nu_vec3f_copy(NU_VEC3F_UP, info.transform.translation);
    nu_vec3f_muls(info.transform.translation, 10.0f, info.transform.translation);
    nuvk_sdf_instance_create(&info, &handle);
    nu_vec3f_copy(NU_VEC3F_RIGHT, info.transform.translation);
    nu_vec3f_muls(info.transform.translation, 10.0f, info.transform.translation);
    nuvk_sdf_instance_create(&info, &handle);
    nu_vec3f_copy(NU_VEC3F_BACKWARD, info.transform.translation);
    nu_vec3f_muls(info.transform.translation, 10.0f, info.transform.translation);
    nuvk_sdf_instance_create(&info, &handle);
    
    nu_vec3f_zero(info.transform.translation);

    // nuvk_sdf_torus_data_t torus_data;
    // torus_data.x = 1.0f;
    // torus_data.y = 0.1;
    // info.type                     = primitives.torus;
    // info.data                     = &torus_data;
    // nu_quatf_from_axis(nu_radian(45.0f), NU_VEC3F_LEFT, info.transform.rotation);
    // nuvk_sdf_instance_create(&info, &handle);

    nuvk_sdf_plane_data_t plane_data;
    nu_vec3f_copy((nu_vec3f_t){0.0f, 1.0f, 0.0f}, plane_data.normal);
    plane_data.height = -5.0f;
    info.type                     = primitives.menger_sponge;
    info.data                     = &plane_data;
    // nu_quatf_from_axis(nu_radian(45.0f), NU_VEC3F_LEFT, info.transform.rotation);
    nuvk_sdf_instance_create(&info, &handle);

    return NU_SUCCESS;
}
nu_result_t nuvk_sdf_renderer_terminate(
    nuvk_sdf_renderer_t *renderer,
    const nuvk_context_t *context,
    const nuvk_memory_manager_t *memory_manager
)
{
    vkDeviceWaitIdle(context->device);

    nuvk_sdf_scene_terminate(&renderer->scene);

    nuvk_sdf_pipelines_terminate(&renderer->pipelines, context);
    nuvk_sdf_descriptors_terminate(&renderer->descriptors, context);
    nuvk_sdf_framebuffers_terminate(&renderer->framebuffers, context);
    nuvk_sdf_renderpasses_terminate(&renderer->renderpasses, context);
    nuvk_sdf_images_terminate(&renderer->images, context, memory_manager);
    nuvk_sdf_buffers_terminate(&renderer->buffers, memory_manager);

    return NU_SUCCESS;
}
nu_result_t nuvk_sdf_renderer_render(
    nuvk_sdf_renderer_t *renderer,
    const nuvk_swapchain_t *swapchain,
    const nuvk_render_context_t *render_context
)
{
    /* recover active command buffer */
    VkCommandBuffer cmd = render_context->command_buffers[render_context->active_inflight_frame_index];

    /* start camera */
    nuvk_sdf_camera_start_frame(&renderer->scene.camera, swapchain);

    /* update scene */
    nuvk_sdf_scene_update_buffers(&renderer->scene, render_context, &renderer->buffers.environment, &renderer->buffers.instances);

    /* screen render area */
    VkRect2D render_area;
    render_area.offset.x = 0;
    render_area.offset.y = 0;
    render_area.extent   = swapchain->extent;

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

    /* set viewport */
    vkCmdSetViewport(cmd, 0, 1, &viewport);
    vkCmdSetScissor(cmd, 0, 1, &scissor);

    /* bind low frequency descriptor set */
    uint32_t dynamic_offsets[3] = {
        renderer->buffers.environment.uniform_buffer_range        * render_context->active_inflight_frame_index,
        renderer->buffers.instances.index_uniform_buffer_range    * render_context->active_inflight_frame_index,
        renderer->buffers.instances.instance_uniform_buffer_range * render_context->active_inflight_frame_index
    };
    vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, renderer->pipelines.geometry.layout,
        0, 1, &renderer->descriptors.low_frequency.descriptor, 3, dynamic_offsets);

    /* geometry pass */
    VkRenderPassBeginInfo geometry_begin_info;
    memset(&geometry_begin_info, 0, sizeof(VkRenderPassBeginInfo));
    geometry_begin_info.sType       = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    geometry_begin_info.renderPass  = renderer->renderpasses.geometry;
    geometry_begin_info.framebuffer = renderer->framebuffers.geometry.framebuffer;
    geometry_begin_info.renderArea  = render_area;
    vkCmdBeginRenderPass(cmd, &geometry_begin_info, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, renderer->pipelines.geometry.pipeline);
    vkCmdDraw(cmd, 3, 1, 0, 0);

    vkCmdEndRenderPass(cmd);

    /* postprocess pass */
    VkRenderPassBeginInfo postprocess_begin_info;
    memset(&postprocess_begin_info, 0, sizeof(VkRenderPassBeginInfo));
    postprocess_begin_info.sType       = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    postprocess_begin_info.renderPass  = renderer->renderpasses.postprocess;
    postprocess_begin_info.framebuffer = renderer->framebuffers.postprocess.framebuffers[render_context->active_swapchain_image_index];
    postprocess_begin_info.renderArea  = render_area;
    vkCmdBeginRenderPass(cmd, &postprocess_begin_info, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, renderer->pipelines.postprocess.pipeline);
    vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, renderer->pipelines.postprocess.layout,
        1, 1, &renderer->descriptors.postprocess.descriptor, 0, NULL);
    vkCmdDraw(cmd, 3, 1, 0, 0);

    vkCmdEndRenderPass(cmd);

    /* end camera */
    nuvk_sdf_camera_end_frame(&renderer->scene.camera);

    return NU_SUCCESS;
}
nu_result_t nuvk_sdf_renderer_update_swapchain(
    nuvk_sdf_renderer_t *renderer,
    const nuvk_context_t *context,
    const nuvk_memory_manager_t *memory_manager,
    const nuvk_swapchain_t *swapchain
)
{
    if (nuvk_sdf_images_update_swapchain(&renderer->images, context, memory_manager, swapchain) != NU_SUCCESS) {
        nu_error(NUVK_LOGGER_NAME"Failed to update swapchain for images.\n");
        return NU_FAILURE;
    }

    if (nuvk_sdf_renderpasses_update_swapchain(&renderer->renderpasses, context, swapchain, &renderer->images) != NU_SUCCESS) {
        nu_error(NUVK_LOGGER_NAME"Failed to update swapchain for pipelines.\n");
        return NU_FAILURE;
    }

    if (nuvk_sdf_framebuffers_update_swapchain(&renderer->framebuffers, context, swapchain, &renderer->images, &renderer->renderpasses) != NU_SUCCESS) {
        nu_error(NUVK_LOGGER_NAME"Failed to update swapchain for framebuffers.\n");
        return NU_FAILURE;
    }

    if (nuvk_sdf_descriptor_update_swapchain(&renderer->descriptors, context, &renderer->images) != NU_SUCCESS) {
        nu_error(NUVK_LOGGER_NAME"Failed to update swapchain for descriptors.\n");
        return NU_FAILURE;
    }

    return NU_SUCCESS;
}
nu_result_t nuvk_sdf_renderer_register_instance_type(
    nuvk_sdf_renderer_t *renderer,
    const nuvk_context_t *context,
    const nuvk_shader_manager_t *shader_manager,
    const nuvk_swapchain_t *swapchain,
    const nuvk_sdf_instance_type_info_t *info,
    nuvk_sdf_instance_type_t *handle
)
{
    vkDeviceWaitIdle(context->device);

    /* add type to the scene */
    nuvk_sdf_scene_register_instance_type(&renderer->scene, info, handle);

    /* build new list of types */
    nuvk_sdf_instance_type_info_t *types = (nuvk_sdf_instance_type_info_t*)nu_malloc(sizeof(nuvk_sdf_instance_type_info_t) * renderer->scene.type_count);
    for (uint32_t i = 0; i < renderer->scene.type_count; i++) {
        types[i] = renderer->scene.types[i].info;
    }

    nu_result_t result = NU_SUCCESS;

    /* reconfigure buffers */
    result &= nuvk_sdf_buffer_instances_configure_instance_types(&renderer->buffers.instances, types, renderer->scene.type_count);
    /* recompile pipeline */
    result &= nuvk_sdf_pipeline_geometry_recompile(&renderer->pipelines.geometry, context, shader_manager, renderer->renderpasses.geometry, 
        renderer->pipelines.sources, types, renderer->scene.type_count);

    /* free resources */
    nu_free(types);

    return result;
}