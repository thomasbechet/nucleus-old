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
        nu_error(NUVK_LOGGER_NAME, "Failed to create buffers.");
        return NU_FAILURE;
    }

    if (nuvk_sdf_images_initialize(&renderer->images, context, memory_manager, swapchain) != NU_SUCCESS) {
        nu_error(NUVK_LOGGER_NAME, "Failed to create images.");
        return NU_FAILURE;
    }

    if (nuvk_sdf_renderpasses_initialize(&renderer->renderpasses, context, swapchain, &renderer->images) != NU_SUCCESS) {
        nu_error(NUVK_LOGGER_NAME, "Failed to create renderpasses.");
        return NU_FAILURE;
    }

    if (nuvk_sdf_framebuffers_initialize(&renderer->framebuffers, context, swapchain, &renderer->images, &renderer->renderpasses) != NU_SUCCESS) {
        nu_error(NUVK_LOGGER_NAME, "Failed to create framebuffers.");
        return NU_FAILURE;
    }

    if (nuvk_sdf_descriptors_initialize(&renderer->descriptors, context, &renderer->buffers, &renderer->images) != NU_SUCCESS) {
        nu_error(NUVK_LOGGER_NAME, "Failed to create descriptors.");
        return NU_FAILURE;
    }

    if (nuvk_sdf_pipelines_initialize(&renderer->pipelines, context, shader_manager, &renderer->descriptors, &renderer->renderpasses) != NU_SUCCESS) {
        nu_error(NUVK_LOGGER_NAME, "Failed to create pipelines.");
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
    const nuvk_context_t *context,
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

    /* transition geometry and light image */
    VkImageMemoryBarrier geometry_light_images[2];
    memset(&geometry_light_images, 0, sizeof(VkImageMemoryBarrier) * 2);
    geometry_light_images[0].sType                           = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    geometry_light_images[0].srcAccessMask                   = VK_ACCESS_SHADER_WRITE_BIT;
    geometry_light_images[0].dstAccessMask                   = VK_ACCESS_SHADER_READ_BIT;
    geometry_light_images[0].oldLayout                       = VK_IMAGE_LAYOUT_GENERAL;
    geometry_light_images[0].newLayout                       = VK_IMAGE_LAYOUT_GENERAL;
    geometry_light_images[0].srcQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED;
    geometry_light_images[0].dstQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED;
    geometry_light_images[0].image                           = renderer->images.geometry.image.image;
    geometry_light_images[0].subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
    geometry_light_images[0].subresourceRange.baseMipLevel   = 0;
    geometry_light_images[0].subresourceRange.levelCount     = 1;
    geometry_light_images[0].subresourceRange.baseArrayLayer = 0;
    geometry_light_images[0].subresourceRange.layerCount     = 1;

    geometry_light_images[1].sType                           = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    geometry_light_images[1].srcAccessMask                   = 0;
    geometry_light_images[1].dstAccessMask                   = VK_ACCESS_SHADER_WRITE_BIT;
    geometry_light_images[1].oldLayout                       = VK_IMAGE_LAYOUT_UNDEFINED;
    geometry_light_images[1].newLayout                       = VK_IMAGE_LAYOUT_GENERAL;
    geometry_light_images[1].srcQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED;
    geometry_light_images[1].dstQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED;
    geometry_light_images[1].image                           = renderer->images.light.image.image;
    geometry_light_images[1].subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
    geometry_light_images[1].subresourceRange.baseMipLevel   = 0;
    geometry_light_images[1].subresourceRange.levelCount     = 1;
    geometry_light_images[1].subresourceRange.baseArrayLayer = 0;
    geometry_light_images[1].subresourceRange.layerCount     = 1;

    vkCmdPipelineBarrier(
        cmd, 
        VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 
        VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
        0, 0, NULL, 0, NULL,
        2, geometry_light_images
    );

    /* light pass */
    vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_COMPUTE, renderer->pipelines.geometry.layout,
        0, 1, &renderer->descriptors.low_frequency.descriptor, 3, dynamic_offsets); 
    vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_COMPUTE, renderer->pipelines.light.layout,
        1, 1, &renderer->descriptors.light.descriptor, 0, NULL);
    vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_COMPUTE, renderer->pipelines.light.pipeline);
    vkCmdDispatch(cmd, (swapchain->extent.width / 16), (swapchain->extent.height / 16), 1);

    VkImageMemoryBarrier light_images;
    memset(&light_images, 0, sizeof(VkImageMemoryBarrier));
    light_images.sType                           = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    light_images.srcAccessMask                   = VK_ACCESS_SHADER_WRITE_BIT;
    light_images.dstAccessMask                   = VK_ACCESS_SHADER_READ_BIT;
    light_images.oldLayout                       = VK_IMAGE_LAYOUT_GENERAL;
    light_images.newLayout                       = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    light_images.srcQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED;
    light_images.dstQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED;
    light_images.image                           = renderer->images.light.image.image;
    light_images.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
    light_images.subresourceRange.baseMipLevel   = 0;
    light_images.subresourceRange.levelCount     = 1;
    light_images.subresourceRange.baseArrayLayer = 0;
    light_images.subresourceRange.layerCount     = 1;

    vkCmdPipelineBarrier(
        cmd, 
        VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT, 
        VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
        0, 0, NULL, 0, NULL,
        1, &light_images
    );

    /* postprocess pass */
    VkRenderPassBeginInfo postprocess_begin_info;
    memset(&postprocess_begin_info, 0, sizeof(VkRenderPassBeginInfo));
    postprocess_begin_info.sType       = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    postprocess_begin_info.renderPass  = renderer->renderpasses.postprocess;
    postprocess_begin_info.framebuffer = renderer->framebuffers.postprocess.framebuffers[render_context->active_swapchain_image_index];
    postprocess_begin_info.renderArea  = render_area;
    vkCmdBeginRenderPass(cmd, &postprocess_begin_info, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, renderer->pipelines.postprocess.layout,
        1, 1, &renderer->descriptors.postprocess.descriptor, 0, NULL);
    vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, renderer->pipelines.postprocess.pipeline);
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
        nu_error(NUVK_LOGGER_NAME, "Failed to update swapchain for images.");
        return NU_FAILURE;
    }

    if (nuvk_sdf_renderpasses_update_swapchain(&renderer->renderpasses, context, swapchain, &renderer->images) != NU_SUCCESS) {
        nu_error(NUVK_LOGGER_NAME, "Failed to update swapchain for pipelines.");
        return NU_FAILURE;
    }

    if (nuvk_sdf_framebuffers_update_swapchain(&renderer->framebuffers, context, swapchain, &renderer->images, &renderer->renderpasses) != NU_SUCCESS) {
        nu_error(NUVK_LOGGER_NAME, "Failed to update swapchain for framebuffers.");
        return NU_FAILURE;
    }

    if (nuvk_sdf_descriptor_update_swapchain(&renderer->descriptors, context, &renderer->images) != NU_SUCCESS) {
        nu_error(NUVK_LOGGER_NAME, "Failed to update swapchain for descriptors.");
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