#include <nucleus/module/vulkan/sdf/renderer.h>

#include <nucleus/module/vulkan/core/image.h>

nu_result_t nuvk_sdf_renderer_initialize(
    nuvk_sdf_renderer_t *renderer,
    const nuvk_context_t *context,
    const nuvk_memory_manager_t *memory_manager,
    const nuvk_shader_manager_t *shader_manager,
    const nuvk_swapchain_t *swapchain,
    const nuvk_render_context_t *render_context
)
{
    nuvk_sdf_camera_initialize(&renderer->camera);
    nuvk_sdf_primitives_initialize(&renderer->primitives);

    if (nuvk_sdf_buffers_initialize(&renderer->buffers, context, memory_manager, render_context, &renderer->primitives) != NU_SUCCESS) {
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

    if (nuvk_sdf_shaders_initialize(&renderer->shaders, context, shader_manager, &renderer->descriptors) != NU_SUCCESS) {
        nu_error(NUVK_LOGGER_NAME"Failed to create shaders.\n");
        return NU_FAILURE;
    }

    if (nuvk_sdf_pipelines_initialize(&renderer->pipelines, context, swapchain, &renderer->shaders, &renderer->renderpasses) != NU_SUCCESS) {
        nu_error(NUVK_LOGGER_NAME"Failed to create pipelines.\n");
        return NU_FAILURE;
    }

    return NU_SUCCESS;
}
nu_result_t nuvk_sdf_renderer_terminate(
    nuvk_sdf_renderer_t *renderer,
    const nuvk_context_t *context,
    const nuvk_memory_manager_t *memory_manager
)
{
    vkDeviceWaitIdle(context->device);

    nuvk_sdf_pipelines_terminate(&renderer->pipelines, context);
    nuvk_sdf_descriptors_terminate(&renderer->descriptors, context);
    nuvk_sdf_shaders_terminate(&renderer->shaders, context);
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
    VkRenderPassBeginInfo begin_info;

    /* recover active command buffer */
    VkCommandBuffer cmd = render_context->command_buffers[render_context->active_inflight_frame_index];

    /* start camera */
    nuvk_sdf_camera_start_frame(&renderer->camera, swapchain);

    /* write environment buffer */
    nuvk_sdf_buffer_environment_write_camera(&renderer->buffers.environment, &renderer->camera,
        render_context->active_inflight_frame_index);

    /* screen render area */
    VkRect2D render_area;
    render_area.offset.x = 0;
    render_area.offset.y = 0;
    render_area.extent   = swapchain->extent;

    /* bind low frequency descriptor set */
    uint32_t dynamic_offsets[3] = {
        renderer->buffers.environment.uniform_buffer_size * render_context->active_inflight_frame_index,
        renderer->buffers.instances.total_header_buffer_size * render_context->active_inflight_frame_index,
        renderer->buffers.instances.total_type_arrays_buffer_size * render_context->active_inflight_frame_index
    };
    vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, renderer->shaders.geometry.layout,
        0, 1, &renderer->descriptors.low_frequency.descriptor, 3, dynamic_offsets);

    /* geometry pass */
    memset(&begin_info, 0, sizeof(VkRenderPassBeginInfo));
    begin_info.sType       = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    begin_info.renderPass  = renderer->renderpasses.geometry;
    begin_info.framebuffer = renderer->framebuffers.geometry.framebuffer;
    begin_info.renderArea  = render_area;
    vkCmdBeginRenderPass(cmd, &begin_info, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, renderer->pipelines.geometry);
    vkCmdDraw(cmd, 3, 1, 0, 0);

    vkCmdEndRenderPass(cmd);

    /* postprocess pass */
    memset(&begin_info, 0, sizeof(VkRenderPassBeginInfo));
    begin_info.sType       = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    begin_info.renderPass  = renderer->renderpasses.postprocess;
    begin_info.framebuffer = renderer->framebuffers.postprocess.framebuffers[render_context->active_swapchain_image_index];
    begin_info.renderArea  = render_area;
    vkCmdBeginRenderPass(cmd, &begin_info, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, renderer->pipelines.postprocess);
    vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, renderer->shaders.postprocess.layout,
        1, 1, &renderer->descriptors.postprocess.descriptor, 0, NULL);
    vkCmdDraw(cmd, 3, 1, 0, 0);

    vkCmdEndRenderPass(cmd);

    /* end camera */
    nuvk_sdf_camera_end_frame(&renderer->camera);

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

    if (nuvk_sdf_pipelines_update_swapchain(&renderer->pipelines, context, swapchain, &renderer->shaders, &renderer->renderpasses) != NU_SUCCESS) {
        nu_error(NUVK_LOGGER_NAME"Failed to update swapchain for pipelines.\n");
        return NU_FAILURE;
    }

    if (nuvk_sdf_descriptor_update_swapchain(&renderer->descriptors, context, &renderer->images) != NU_SUCCESS) {
        nu_error(NUVK_LOGGER_NAME"Failed to update swapchain for descriptors.\n");
        return NU_FAILURE;
    }

    return NU_SUCCESS;
}