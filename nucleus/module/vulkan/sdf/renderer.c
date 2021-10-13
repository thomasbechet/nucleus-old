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
    nu_result_t result;

    nu_vec2u_copy((nu_vec2u_t){1024, 576}, renderer->viewport_size); /* default internal resolution */

    /* buffers */
    result =  nuvk_sdf_buffer_environment_create(&renderer->buffers.environment, context, memory_manager, render_context);
    NU_CHECK(result == NU_SUCCESS, return NU_FAILURE, NUVK_LOGGER_NAME, "Failed to create environment buffer.");
    result = nuvk_sdf_buffer_instances_create(&renderer->buffers.instances, context, memory_manager, render_context);
    NU_CHECK(result == NU_SUCCESS, return NU_FAILURE, NUVK_LOGGER_NAME, "Failed to create instances buffer.");
    result = nuvk_sdf_buffer_materials_create(&renderer->buffers.materials, context, memory_manager, render_context);
    NU_CHECK(result == NU_SUCCESS, return NU_FAILURE, NUVK_LOGGER_NAME, "Failed to create materials buffer.");

    /* images */
    result = nuvk_sdf_image_geometry_create(&renderer->images.geometry, context, memory_manager, renderer->viewport_size);
    NU_CHECK(result == NU_SUCCESS, return result, NUVK_LOGGER_NAME, "Failed to create geometry image.");
    result = nuvk_sdf_image_light_create(&renderer->images.light, context, memory_manager, renderer->viewport_size);
    NU_CHECK(result == NU_SUCCESS, return result, NUVK_LOGGER_NAME, "Failed to create light image.");

    /* renderpasses */
    result = nuvk_sdf_renderpass_geometry_create(&renderer->renderpasses.geometry, context, &renderer->images.geometry);
    NU_CHECK(result == NU_SUCCESS, return result, NUVK_LOGGER_NAME, "Failed to create geometry renderpass.");
    result = nuvk_sdf_renderpass_postprocess_create(&renderer->renderpasses.postprocess, context, swapchain);
    NU_CHECK(result == NU_SUCCESS, return result, NUVK_LOGGER_NAME, "Failed to create postprocess renderpass.");

    /* framebuffers */
    result = nuvk_sdf_framebuffer_geometry_create(&renderer->framebuffers.geometry, context, &renderer->images.geometry, renderer->renderpasses.geometry);
    NU_CHECK(result == NU_SUCCESS, return NU_FAILURE, NUVK_LOGGER_NAME, "Failed to create geometry framebuffer.");
    result = nuvk_sdf_framebuffer_postprocess_create(&renderer->framebuffers.postprocess, context, swapchain, renderer->renderpasses.postprocess);
    NU_CHECK(result == NU_SUCCESS, return NU_FAILURE, NUVK_LOGGER_NAME, "Failed to create postprocess framebuffer.");

    /* descriptors */
    result = nuvk_sdf_descriptor_pool_create(&renderer->descriptors.pool, context, 2);
    NU_CHECK(result == NU_SUCCESS, return NU_FAILURE, NUVK_LOGGER_NAME, "Failed to create descriptor pool.");
    result = nuvk_sdf_descriptor_low_frequency_create(&renderer->descriptors.low_frequency, context, 
        &renderer->buffers.environment, &renderer->buffers.instances, &renderer->buffers.materials, renderer->descriptors.pool);
    NU_CHECK(result == NU_SUCCESS, return NU_FAILURE, NUVK_LOGGER_NAME, "Failed to create low frequency descriptor.");
    result = nuvk_sdf_descriptor_light_create(&renderer->descriptors.light, context,
        &renderer->images.geometry, &renderer->images.light, renderer->descriptors.pool);
    NU_CHECK(result == NU_SUCCESS, return NU_FAILURE, NUVK_LOGGER_NAME, "Failed to create light descriptor.");
    result = nuvk_sdf_descriptor_postprocess_create(&renderer->descriptors.postprocess, context,
        &renderer->images.light, renderer->descriptors.pool);
    NU_CHECK(result == NU_SUCCESS, return NU_FAILURE, NUVK_LOGGER_NAME, "Failed to create postprocess descriptor.");

    /* pipelines */
    result = nuvk_sdf_pipeline_generator_initialize(&renderer->pipelines.generator);
    NU_CHECK(result == NU_SUCCESS, return result, NUVK_LOGGER_NAME, "Failed to create pipeline generator.");
    result = nuvk_sdf_pipeline_geometry_create(&renderer->pipelines.geometry, context,
        shader_manager, &renderer->descriptors, renderer->renderpasses.geometry, &renderer->pipelines.generator);
    NU_CHECK(result == NU_SUCCESS, return result, NUVK_LOGGER_NAME, "Failed to create geometry pipeline.");
    result = nuvk_sdf_pipeline_light_create(&renderer->pipelines.light, context,
        shader_manager, &renderer->descriptors, &renderer->pipelines.generator);
    NU_CHECK(result == NU_SUCCESS, return result, NUVK_LOGGER_NAME, "Failed to create light pipeline.");
    result = nuvk_sdf_pipeline_postprocess_create(&renderer->pipelines.postprocess, context,
        shader_manager, &renderer->descriptors, renderer->renderpasses.postprocess, &renderer->pipelines.generator);
    NU_CHECK(result == NU_SUCCESS, return result, NUVK_LOGGER_NAME, "Failed to create postprocess pipeline.");

    /* create scene and primitives */
    nuvk_sdf_scene_initialize(&renderer->scene);
    nuvk_sdf_primitives_register(renderer->primitives);
    
    // nuvk_sdf_instance_info_t info;
    // info.type  = primitives.sphere;
    // info.flags = 0x0;
    // nu_transform_identity(&info.transform);
    // nuvk_sdf_sphere_data_t data;
    // data.radius = 1.0f;
    // info.data = &data;
    // nuvk_sdf_instance_t handle;
    
    // nu_vec3f_copy(NU_VEC3F_UP, info.transform.translation);
    // nu_vec3f_muls(info.transform.translation, 10.0f, info.transform.translation);
    // nuvk_sdf_instance_create(&info, &handle);
    // nu_vec3f_copy(NU_VEC3F_RIGHT, info.transform.translation);
    // nu_vec3f_muls(info.transform.translation, 10.0f, info.transform.translation);
    // nuvk_sdf_instance_create(&info, &handle);
    // nu_vec3f_copy(NU_VEC3F_BACKWARD, info.transform.translation);
    // nu_vec3f_muls(info.transform.translation, 10.0f, info.transform.translation);
    // nuvk_sdf_instance_create(&info, &handle);
    
    // nu_vec3f_zero(info.transform.translation);

    // nuvk_sdf_torus_data_t torus_data;
    // torus_data.x = 1.0f;
    // torus_data.y = 0.1;
    // info.type                     = primitives.torus;
    // info.data                     = &torus_data;
    // nu_quatf_from_axis(nu_radian(45.0f), NU_VEC3F_LEFT, info.transform.rotation);
    // nuvk_sdf_instance_create(&info, &handle);

    // nuvk_sdf_plane_data_t plane_data;
    // nu_vec3f_copy((nu_vec3f_t){0.0f, 1.0f, 0.0f}, plane_data.normal);
    // plane_data.height = -5.0f;
    // info.type                     = primitives.menger_sponge;
    // info.data                     = &plane_data;
    // // nu_quatf_from_axis(nu_radian(45.0f), NU_VEC3F_LEFT, info.transform.rotation);
    // nuvk_sdf_instance_create(&info, &handle);

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

    /* pipelines */
    nuvk_sdf_pipeline_geometry_destroy(&renderer->pipelines.geometry, context);
    nuvk_sdf_pipeline_light_destroy(&renderer->pipelines.light, context);
    nuvk_sdf_pipeline_postprocess_destroy(&renderer->pipelines.postprocess, context);
    nuvk_sdf_pipeline_generator_terminate(&renderer->pipelines.generator);

    /* descriptors */
    nuvk_sdf_descriptor_light_destroy(&renderer->descriptors.light, context);
    nuvk_sdf_descriptor_postprocess_destroy(&renderer->descriptors.postprocess, context);
    nuvk_sdf_descriptor_low_frequency_destroy(&renderer->descriptors.low_frequency, context);
    vkDestroyDescriptorPool(context->device, renderer->descriptors.pool, &context->allocator);
    
    /* framebuffers */
    nuvk_sdf_framebuffer_postprocess_destroy(&renderer->framebuffers.postprocess, context);
    nuvk_sdf_framebuffer_geometry_destroy(&renderer->framebuffers.geometry, context);
    
    /* renderpasses */
    vkDestroyRenderPass(context->device, renderer->renderpasses.postprocess, &context->allocator);
    vkDestroyRenderPass(context->device, renderer->renderpasses.geometry, &context->allocator);
    
    /* images */
    nuvk_sdf_image_light_destroy(&renderer->images.light, context, memory_manager);
    nuvk_sdf_image_geometry_destroy(&renderer->images.geometry, context, memory_manager);
    
    /* buffers */
    nuvk_sdf_buffer_materials_destroy(&renderer->buffers.materials, memory_manager);
    nuvk_sdf_buffer_instances_destroy(&renderer->buffers.instances, memory_manager);
    nuvk_sdf_buffer_environment_destroy(&renderer->buffers.environment, memory_manager);

    return NU_SUCCESS;
}
nu_result_t nuvk_sdf_renderer_update_swapchain(
    nuvk_sdf_renderer_t *renderer,
    const nuvk_context_t *context,
    const nuvk_memory_manager_t *memory_manager,
    const nuvk_swapchain_t *swapchain
)
{
    nu_result_t result;

    /* renderpasses */
    vkDestroyRenderPass(context->device, renderer->renderpasses.postprocess, &context->allocator);
    result = nuvk_sdf_renderpass_postprocess_create(&renderer->renderpasses.postprocess, context, swapchain);
    NU_CHECK(result == NU_SUCCESS, return result, NUVK_LOGGER_NAME, "Failed to recreate postprocess renderpass.");

    /* framebuffers */
    nuvk_sdf_framebuffer_postprocess_destroy(&renderer->framebuffers.postprocess, context);
    result = nuvk_sdf_framebuffer_postprocess_create(&renderer->framebuffers.postprocess, context, swapchain, renderer->renderpasses.postprocess);
    NU_CHECK(result == NU_SUCCESS, return NU_FAILURE, NUVK_LOGGER_NAME, "Failed to recreate postprocess framebuffer.");

    return NU_SUCCESS;
}
nu_result_t nuvk_sdf_renderer_update_viewport_size(
    nuvk_sdf_renderer_t *renderer,
    const nuvk_context_t *context,
    const nuvk_memory_manager_t *memory_manager,
    const nu_vec2u_t size
)
{
    nu_result_t result;

    vkDeviceWaitIdle(context->device);

    nu_vec2u_copy(size, renderer->viewport_size);

    /* images */
    nuvk_sdf_image_geometry_destroy(&renderer->images.geometry, context, memory_manager);
    result = nuvk_sdf_image_geometry_create(&renderer->images.geometry, context, memory_manager, renderer->viewport_size);
    NU_CHECK(result == NU_SUCCESS, return result, NUVK_LOGGER_NAME, "Failed to create geometry image.");

    nuvk_sdf_image_light_destroy(&renderer->images.light, context, memory_manager);
    result = nuvk_sdf_image_light_create(&renderer->images.light, context, memory_manager, renderer->viewport_size);
    NU_CHECK(result == NU_SUCCESS, return result, NUVK_LOGGER_NAME, "Failed to create light image.");

    /* framebuffers */
    nuvk_sdf_framebuffer_geometry_destroy(&renderer->framebuffers.geometry, context);
    result = nuvk_sdf_framebuffer_geometry_create(&renderer->framebuffers.geometry, context, &renderer->images.geometry, renderer->renderpasses.geometry);
    NU_CHECK(result == NU_SUCCESS, return NU_FAILURE, NUVK_LOGGER_NAME, "Failed to create geometry framebuffer.");

    /* descriptors */
    result = nuvk_sdf_descriptor_light_update_images(&renderer->descriptors.light, context, &renderer->images.geometry, &renderer->images.light);
    NU_CHECK(result == NU_SUCCESS, return NU_FAILURE, NUVK_LOGGER_NAME, "Failed to update light descriptor.");

    result = nuvk_sdf_descriptor_postprocess_update_images(&renderer->descriptors.postprocess, context, &renderer->images.light);
    NU_CHECK(result == NU_SUCCESS, return NU_FAILURE, NUVK_LOGGER_NAME, "Failed to update postprocess descriptor.");
    
    return NU_SUCCESS;
}
nu_result_t nuvk_sdf_renderer_register_instance_type(
    nuvk_sdf_renderer_t *renderer,
    const nuvk_context_t *context,
    const nuvk_shader_manager_t *shader_manager,
    const nuvk_render_context_t *render_context,
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

    nu_result_t result;

    /* reconfigure buffers */
    result = nuvk_sdf_buffer_instances_configure_instance_types(&renderer->buffers.instances, types, renderer->scene.type_count);
    NU_CHECK(result == NU_SUCCESS, goto cleanup0, NUVK_LOGGER_NAME, "Failed to reconfigure buffers.");

    /* write initial buffer value length to 0 */
    nuvk_sdf_buffer_instances_write_indices(&renderer->buffers.instances, render_context->active_inflight_frame_index, 
        renderer->scene.type_count - 1, NULL, 0);
    
    /* recompile pipelines */
    result = nuvk_sdf_pipeline_generator_update_instance_types(&renderer->pipelines.generator, types, renderer->scene.type_count);
    NU_CHECK(result == NU_SUCCESS, goto cleanup0, NUVK_LOGGER_NAME, "Failed to update pipeline generator.");
    result = nuvk_sdf_pipeline_geometry_recompile(&renderer->pipelines.geometry, context, shader_manager, 
        renderer->renderpasses.geometry, &renderer->pipelines.generator);
    NU_CHECK(result == NU_SUCCESS, goto cleanup0, NUVK_LOGGER_NAME, "Failed to recompile geometry pipeline.");
    result = nuvk_sdf_pipeline_light_recompile(&renderer->pipelines.light, context, shader_manager, 
        &renderer->descriptors, &renderer->pipelines.generator);
    NU_CHECK(result == NU_SUCCESS, goto cleanup0, NUVK_LOGGER_NAME, "Failed to recompile light pipeline.");

cleanup0:
    nu_free(types);
    return result;
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
    nuvk_sdf_camera_start_frame(&renderer->scene.camera, renderer->viewport_size);

    /* update scene */
    nuvk_sdf_scene_update_buffers(&renderer->scene, render_context, 
        &renderer->buffers.environment, &renderer->buffers.instances, &renderer->buffers.materials);

    /* viewport render area */
    VkRect2D render_area;
    render_area.offset.x      = 0;
    render_area.offset.y      = 0;
    render_area.extent.width  = renderer->viewport_size[0];
    render_area.extent.height = renderer->viewport_size[1];

    VkViewport viewport;
    viewport.x        = 0;
    viewport.y        = renderer->viewport_size[1];
    viewport.width    = renderer->viewport_size[0];
    viewport.height   = -(int32_t)renderer->viewport_size[1];
    viewport.minDepth = 0.0;
    viewport.maxDepth = 1.0;

    VkRect2D scissor;
    scissor.offset.x      = 0;
    scissor.offset.y      = 0;
    scissor.extent.width  = renderer->viewport_size[0];
    scissor.extent.height = renderer->viewport_size[1];

    vkCmdSetViewport(cmd, 0, 1, &viewport);
    vkCmdSetScissor(cmd, 0, 1, &scissor);

    /* bind low frequency descriptor set */
    uint32_t dynamic_offsets[4] = {
        renderer->buffers.environment.uniform_buffer_range        * render_context->active_inflight_frame_index,
        renderer->buffers.instances.index_uniform_buffer_range    * render_context->active_inflight_frame_index,
        renderer->buffers.instances.instance_uniform_buffer_range * render_context->active_inflight_frame_index,
        renderer->buffers.materials.uniform_buffer_range          * render_context->active_inflight_frame_index
    };
    vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, renderer->pipelines.geometry.layout,
        0, 1, &renderer->descriptors.low_frequency.descriptor, 4, dynamic_offsets);

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
    VkImageMemoryBarrier geometry_light_images[3];
    memset(&geometry_light_images, 0, sizeof(VkImageMemoryBarrier) * 3);

    geometry_light_images[0].sType                           = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    geometry_light_images[0].srcAccessMask                   = VK_ACCESS_SHADER_WRITE_BIT;
    geometry_light_images[0].dstAccessMask                   = VK_ACCESS_SHADER_READ_BIT;
    geometry_light_images[0].oldLayout                       = VK_IMAGE_LAYOUT_GENERAL;
    geometry_light_images[0].newLayout                       = VK_IMAGE_LAYOUT_GENERAL;
    geometry_light_images[0].srcQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED;
    geometry_light_images[0].dstQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED;
    geometry_light_images[0].image                           = renderer->images.geometry.normal_depth.image;
    geometry_light_images[0].subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
    geometry_light_images[0].subresourceRange.baseMipLevel   = 0;
    geometry_light_images[0].subresourceRange.levelCount     = 1;
    geometry_light_images[0].subresourceRange.baseArrayLayer = 0;
    geometry_light_images[0].subresourceRange.layerCount     = 1;

    geometry_light_images[1].sType                           = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    geometry_light_images[1].srcAccessMask                   = VK_ACCESS_SHADER_WRITE_BIT;
    geometry_light_images[1].dstAccessMask                   = VK_ACCESS_SHADER_READ_BIT;
    geometry_light_images[1].oldLayout                       = VK_IMAGE_LAYOUT_GENERAL;
    geometry_light_images[1].newLayout                       = VK_IMAGE_LAYOUT_GENERAL;
    geometry_light_images[1].srcQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED;
    geometry_light_images[1].dstQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED;
    geometry_light_images[1].image                           = renderer->images.geometry.position_material.image;
    geometry_light_images[1].subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
    geometry_light_images[1].subresourceRange.baseMipLevel   = 0;
    geometry_light_images[1].subresourceRange.levelCount     = 1;
    geometry_light_images[1].subresourceRange.baseArrayLayer = 0;
    geometry_light_images[1].subresourceRange.layerCount     = 1;

    geometry_light_images[2].sType                           = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    geometry_light_images[2].srcAccessMask                   = 0;
    geometry_light_images[2].dstAccessMask                   = VK_ACCESS_SHADER_WRITE_BIT;
    geometry_light_images[2].oldLayout                       = VK_IMAGE_LAYOUT_UNDEFINED;
    geometry_light_images[2].newLayout                       = VK_IMAGE_LAYOUT_GENERAL;
    geometry_light_images[2].srcQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED;
    geometry_light_images[2].dstQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED;
    geometry_light_images[2].image                           = renderer->images.light.image.image;
    geometry_light_images[2].subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
    geometry_light_images[2].subresourceRange.baseMipLevel   = 0;
    geometry_light_images[2].subresourceRange.levelCount     = 1;
    geometry_light_images[2].subresourceRange.baseArrayLayer = 0;
    geometry_light_images[2].subresourceRange.layerCount     = 1;

    vkCmdPipelineBarrier(
        cmd, 
        VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 
        VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
        0, 0, NULL, 0, NULL,
        3, geometry_light_images
    );

    /* light pass */
    vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_COMPUTE, renderer->pipelines.light.layout,
        0, 1, &renderer->descriptors.low_frequency.descriptor, 4, dynamic_offsets); 
    vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_COMPUTE, renderer->pipelines.light.layout,
        1, 1, &renderer->descriptors.light.descriptor, 0, NULL);
    vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_COMPUTE, renderer->pipelines.light.pipeline);
    vkCmdDispatch(cmd, (renderer->viewport_size[0] / 16), (renderer->viewport_size[1] / 16), 1);

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

    /* screen render area */
    render_area.offset.x = 0;
    render_area.offset.y = 0;
    render_area.extent   = swapchain->extent;

    viewport.x        = 0;
    viewport.y        = swapchain->extent.height;
    viewport.width    = swapchain->extent.width;
    viewport.height   = -(int32_t)swapchain->extent.height;
    viewport.minDepth = 0.0;
    viewport.maxDepth = 1.0;

    scissor = render_area;

    vkCmdSetViewport(cmd, 0, 1, &viewport);
    vkCmdSetScissor(cmd, 0, 1, &scissor);

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