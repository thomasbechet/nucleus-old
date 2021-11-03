#include <nucleus/module/vulkan/renderer.h>

#include <nucleus/module/vulkan/common/logger.h>
#include <nucleus/module/vulkan/glfw/interface.h>
#include <nucleus/module/vulkan/core/context.h>
#include <nucleus/module/vulkan/core/memory.h>
#include <nucleus/module/vulkan/core/command_pool.h>
#include <nucleus/module/vulkan/core/swapchain.h>
#include <nucleus/module/vulkan/core/render_context.h>
#include <nucleus/module/vulkan/core/shader.h>
#include <nucleus/module/vulkan/buffer/buffers.h>
#include <nucleus/module/vulkan/renderpass/renderpasses.h>
#include <nucleus/module/vulkan/descriptor/descriptors.h>
#include <nucleus/module/vulkan/pipeline/pipelines.h>
#include <nucleus/module/vulkan/image/images.h>
#include <nucleus/module/vulkan/framebuffer/framebuffers.h>
#include <nucleus/module/vulkan/scene/scene.h>
#include <nucleus/module/vulkan/scene/primitives.h>
#include <nucleus/module/vulkan/assets/assets.h>

typedef struct {
    nuvk_context_t context;
    nuvk_memory_manager_t memory_manager;
    nuvk_shader_manager_t shader_manager;
    nuvk_command_pool_t command_pool;
    nuvk_swapchain_t swapchain;
    nuvk_render_context_t render_context;
    bool render_context_out_of_date;

    nuvk_buffers_t buffers;
    nuvk_renderpasses_t renderpasses;
    nuvk_descriptors_t descriptors;
    nuvk_pipelines_t pipelines;
    nuvk_images_t images;
    nuvk_framebuffers_t framebuffers;
    nuvk_assets_t assets;
    nuvk_scene_t scene;
    nuvk_sdf_t primitives[32];
    nu_vec2u_t viewport_size;

    nuvk_image_t test_image;
} nuvk_module_data_t;

static nuvk_module_data_t _module;

nu_result_t nuvk_renderer_initialize(void)
{
    _module.render_context_out_of_date = false;
    // nu_vec2u_copy((nu_vec2u_t){1024, 576}, _module.viewport_size); /* default internal resolution */
    nu_vec2u_copy((nu_vec2u_t){1920, 1080}, _module.viewport_size); /* default internal resolution */

    nu_result_t result = nuvk_glfw_get_interface();
    NU_CHECK(result == NU_SUCCESS, return NU_FAILURE, NUVK_LOGGER_NAME, "Failed to get glfw interface.");

    /* core */
    result = nuvk_context_initialize(&_module.context);
    NU_CHECK(result == NU_SUCCESS, return NU_FAILURE, NUVK_LOGGER_NAME, "Failed to initialize context.");
    result = nuvk_memory_manager_initialize(&_module.memory_manager, &_module.context);
    NU_CHECK(result == NU_SUCCESS, return NU_FAILURE, NUVK_LOGGER_NAME, "Failed to initialize memory manager.");
    result = nuvk_shader_manager_initialize(&_module.shader_manager);
    NU_CHECK(result == NU_SUCCESS, return NU_FAILURE, NUVK_LOGGER_NAME, "Failed to initialize shader manager.");
    result = nuvk_command_pool_initialize(&_module.command_pool, &_module.context);
    NU_CHECK(result == NU_SUCCESS, return NU_FAILURE, NUVK_LOGGER_NAME, "Failed to initialize command pool.");
    result = nuvk_swapchain_initialize(&_module.swapchain, &_module.context, UINT32_MAX, UINT32_MAX);
    NU_CHECK(result == NU_SUCCESS, return NU_FAILURE, NUVK_LOGGER_NAME, "Failed to initialize swapchain.");
    result = nuvk_render_context_initialize(&_module.render_context, &_module.context, &_module.swapchain, &_module.command_pool, 3);
    NU_CHECK(result == NU_SUCCESS, return NU_FAILURE, NUVK_LOGGER_NAME, "Failed to initialize render context.");

    /* buffers */
    result =  nuvk_buffer_environment_create(&_module.buffers.environment, &_module.context, &_module.memory_manager, &_module.render_context);
    NU_CHECK(result == NU_SUCCESS, return NU_FAILURE, NUVK_LOGGER_NAME, "Failed to create environment buffer.");
    result = nuvk_buffer_instances_create(&_module.buffers.instances, &_module.context, &_module.memory_manager, &_module.render_context);
    NU_CHECK(result == NU_SUCCESS, return NU_FAILURE, NUVK_LOGGER_NAME, "Failed to create instances buffer.");
    result = nuvk_buffer_materials_create(&_module.buffers.materials, &_module.context, &_module.memory_manager, &_module.render_context);
    NU_CHECK(result == NU_SUCCESS, return NU_FAILURE, NUVK_LOGGER_NAME, "Failed to create materials buffer.");

    /* images */
    result = nuvk_image_geometry_create(&_module.images.geometry, &_module.context, &_module.memory_manager, _module.viewport_size);
    NU_CHECK(result == NU_SUCCESS, return result, NUVK_LOGGER_NAME, "Failed to create geometry image.");
    result = nuvk_image_light_create(&_module.images.light, &_module.context, &_module.memory_manager, _module.viewport_size);
    NU_CHECK(result == NU_SUCCESS, return result, NUVK_LOGGER_NAME, "Failed to create light image.");

    /* renderpasses */
    result = nuvk_renderpass_geometry_create(&_module.renderpasses.geometry, &_module.context, &_module.images.geometry);
    NU_CHECK(result == NU_SUCCESS, return result, NUVK_LOGGER_NAME, "Failed to create geometry renderpass.");
    result = nuvk_renderpass_postprocess_create(&_module.renderpasses.postprocess, &_module.context, &_module.swapchain);
    NU_CHECK(result == NU_SUCCESS, return result, NUVK_LOGGER_NAME, "Failed to create postprocess renderpass.");

    /* framebuffers */
    result = nuvk_framebuffer_geometry_create(&_module.framebuffers.geometry, &_module.context, &_module.images.geometry, _module.renderpasses.geometry);
    NU_CHECK(result == NU_SUCCESS, return NU_FAILURE, NUVK_LOGGER_NAME, "Failed to create geometry framebuffer.");
    result = nuvk_framebuffer_postprocess_create(&_module.framebuffers.postprocess, &_module.context, &_module.swapchain, _module.renderpasses.postprocess);
    NU_CHECK(result == NU_SUCCESS, return NU_FAILURE, NUVK_LOGGER_NAME, "Failed to create postprocess framebuffer.");

    /* descriptors */
    result = nuvk_descriptor_pool_create(&_module.descriptors.pool, &_module.context, 2);
    NU_CHECK(result == NU_SUCCESS, return NU_FAILURE, NUVK_LOGGER_NAME, "Failed to create descriptor pool.");
    result = nuvk_descriptor_low_frequency_create(&_module.descriptors.low_frequency, &_module.context, 
        &_module.buffers.environment, &_module.buffers.instances, &_module.buffers.materials, _module.descriptors.pool);
    NU_CHECK(result == NU_SUCCESS, return NU_FAILURE, NUVK_LOGGER_NAME, "Failed to create low frequency descriptor.");
    result = nuvk_descriptor_light_create(&_module.descriptors.light, &_module.context,
        &_module.images.geometry, &_module.images.light, _module.descriptors.pool);
    NU_CHECK(result == NU_SUCCESS, return NU_FAILURE, NUVK_LOGGER_NAME, "Failed to create light descriptor.");
    result = nuvk_descriptor_postprocess_create(&_module.descriptors.postprocess, &_module.context,
        &_module.images.light, _module.descriptors.pool);
    NU_CHECK(result == NU_SUCCESS, return NU_FAILURE, NUVK_LOGGER_NAME, "Failed to create postprocess descriptor.");

    /* pipelines */
    result = nuvk_pipeline_generator_initialize(&_module.pipelines.generator);
    NU_CHECK(result == NU_SUCCESS, return result, NUVK_LOGGER_NAME, "Failed to create pipeline generator.");
    result = nuvk_pipeline_geometry_create(&_module.pipelines.geometry, &_module.context,
        &_module.shader_manager, &_module.descriptors, _module.renderpasses.geometry, &_module.pipelines.generator);
    NU_CHECK(result == NU_SUCCESS, return result, NUVK_LOGGER_NAME, "Failed to create geometry pipeline.");
    result = nuvk_pipeline_light_create(&_module.pipelines.light, &_module.context,
        &_module.shader_manager, &_module.descriptors, &_module.pipelines.generator);
    NU_CHECK(result == NU_SUCCESS, return result, NUVK_LOGGER_NAME, "Failed to create light pipeline.");
    result = nuvk_pipeline_postprocess_create(&_module.pipelines.postprocess, &_module.context,
        &_module.shader_manager, &_module.descriptors, _module.renderpasses.postprocess, &_module.pipelines.generator);
    NU_CHECK(result == NU_SUCCESS, return result, NUVK_LOGGER_NAME, "Failed to create postprocess pipeline.");

    /* assets */
    result = nuvk_assets_initialize(&_module.assets);
    NU_CHECK(result == NU_SUCCESS, return result, NUVK_LOGGER_NAME, "Failed to initialize assets.");

    /* scene */
    nuvk_scene_initialize(&_module.scene);
    
    /* register primitives */
    nuvk_primitives_register(_module.primitives);

    return NU_SUCCESS;
}
nu_result_t nuvk_renderer_terminate(void)
{
    vkDeviceWaitIdle(_module.context.device);

    /* scene */
    nuvk_scene_terminate(&_module.scene);

    /* assets */
    nuvk_assets_terminate(&_module.assets);

    /* pipelines */
    nuvk_pipeline_geometry_destroy(&_module.pipelines.geometry, &_module.context);
    nuvk_pipeline_light_destroy(&_module.pipelines.light, &_module.context);
    nuvk_pipeline_postprocess_destroy(&_module.pipelines.postprocess, &_module.context);
    nuvk_pipeline_generator_terminate(&_module.pipelines.generator);

    /* descriptors */
    nuvk_descriptor_light_destroy(&_module.descriptors.light, &_module.context);
    nuvk_descriptor_postprocess_destroy(&_module.descriptors.postprocess, &_module.context);
    nuvk_descriptor_low_frequency_destroy(&_module.descriptors.low_frequency, &_module.context);
    vkDestroyDescriptorPool(_module.context.device, _module.descriptors.pool, &_module.context.allocator);
    
    /* framebuffers */
    nuvk_framebuffer_postprocess_destroy(&_module.framebuffers.postprocess, &_module.context);
    nuvk_framebuffer_geometry_destroy(&_module.framebuffers.geometry, &_module.context);
    
    /* renderpasses */
    vkDestroyRenderPass(_module.context.device, _module.renderpasses.postprocess, &_module.context.allocator);
    vkDestroyRenderPass(_module.context.device, _module.renderpasses.geometry, &_module.context.allocator);
    
    /* images */
    nuvk_image_light_destroy(&_module.images.light, &_module.context, &_module.memory_manager);
    nuvk_image_geometry_destroy(&_module.images.geometry, &_module.context, &_module.memory_manager);
    
    /* buffers */
    nuvk_buffer_materials_destroy(&_module.buffers.materials, &_module.memory_manager);
    nuvk_buffer_instances_destroy(&_module.buffers.instances, &_module.memory_manager);
    nuvk_buffer_environment_destroy(&_module.buffers.environment, &_module.memory_manager);

    /* core */
    nuvk_render_context_terminate(&_module.render_context, &_module.context);
    nuvk_swapchain_terminate(&_module.swapchain, &_module.context);
    nuvk_command_pool_terminate(&_module.command_pool, &_module.context);
    nuvk_shader_manager_terminate(&_module.shader_manager);
    nuvk_memory_manager_terminate(&_module.memory_manager);
    nuvk_context_terminate(&_module.context);

    return NU_SUCCESS;
}
static nu_result_t nuvk_renderer_execute_frame(void)
{
    /* recover active command buffer */
    VkCommandBuffer cmd = _module.render_context.command_buffers[_module.render_context.active_inflight_frame_index];

    /* start camera */
    nuvk_camera_start_frame(&_module.scene.camera, _module.viewport_size);
    nuvk_buffer_environment_write_camera(&_module.buffers.environment, &_module.scene.camera,
        _module.render_context.active_inflight_frame_index);

    /* transfer buffers */
    nuvk_dynamic_range_buffer_transfer(&_module.buffers.environment.dynamic_range_buffer, _module.render_context.active_inflight_frame_index);
    nuvk_dynamic_range_buffer_transfer(&_module.buffers.instances.index_buffer,           _module.render_context.active_inflight_frame_index);
    nuvk_dynamic_range_buffer_transfer(&_module.buffers.instances.instance_buffer,        _module.render_context.active_inflight_frame_index);
    nuvk_dynamic_range_buffer_transfer(&_module.buffers.instances.material_buffer,        _module.render_context.active_inflight_frame_index);
    nuvk_dynamic_range_buffer_transfer(&_module.buffers.materials.dynamic_range_buffer,   _module.render_context.active_inflight_frame_index);

    /* viewport render area */
    VkRect2D render_area;
    render_area.offset.x      = 0;
    render_area.offset.y      = 0;
    render_area.extent.width  = _module.viewport_size[0];
    render_area.extent.height = _module.viewport_size[1];

    VkViewport viewport;
    viewport.x        = 0;
    viewport.y        = _module.viewport_size[1];
    viewport.width    = _module.viewport_size[0];
    viewport.height   = -(int32_t)_module.viewport_size[1];
    viewport.minDepth = 0.0;
    viewport.maxDepth = 1.0;

    VkRect2D scissor;
    scissor.offset.x      = 0;
    scissor.offset.y      = 0;
    scissor.extent.width  = _module.viewport_size[0];
    scissor.extent.height = _module.viewport_size[1];

    vkCmdSetViewport(cmd, 0, 1, &viewport);
    vkCmdSetScissor(cmd, 0, 1, &scissor);

    /* bind low frequency descriptor set */
    uint32_t dynamic_offsets[4] = {
        _module.buffers.environment.uniform_buffer_range        * _module.render_context.active_inflight_frame_index,
        _module.buffers.instances.index_uniform_buffer_range    * _module.render_context.active_inflight_frame_index,
        _module.buffers.instances.instance_uniform_buffer_range * _module.render_context.active_inflight_frame_index,
        _module.buffers.materials.uniform_buffer_range          * _module.render_context.active_inflight_frame_index
    };
    vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, _module.pipelines.geometry.layout,
        0, 1, &_module.descriptors.low_frequency.descriptor, 4, dynamic_offsets);

    /* geometry pass */
    VkRenderPassBeginInfo geometry_begin_info;
    memset(&geometry_begin_info, 0, sizeof(VkRenderPassBeginInfo));
    geometry_begin_info.sType       = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    geometry_begin_info.renderPass  = _module.renderpasses.geometry;
    geometry_begin_info.framebuffer = _module.framebuffers.geometry.framebuffer;
    geometry_begin_info.renderArea  = render_area;
    vkCmdBeginRenderPass(cmd, &geometry_begin_info, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, _module.pipelines.geometry.pipeline);
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
    geometry_light_images[0].image                           = _module.images.geometry.normal_depth.image;
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
    geometry_light_images[1].image                           = _module.images.geometry.position_material.image;
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
    geometry_light_images[2].image                           = _module.images.light.image.image;
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
    vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_COMPUTE, _module.pipelines.light.layout,
        0, 1, &_module.descriptors.low_frequency.descriptor, 4, dynamic_offsets); 
    vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_COMPUTE, _module.pipelines.light.layout,
        1, 1, &_module.descriptors.light.descriptor, 0, NULL);
    vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_COMPUTE, _module.pipelines.light.pipeline);
    vkCmdDispatch(cmd, (_module.viewport_size[0] / 16), (_module.viewport_size[1] / 16), 1);

    VkImageMemoryBarrier light_images;
    memset(&light_images, 0, sizeof(VkImageMemoryBarrier));
    light_images.sType                           = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    light_images.srcAccessMask                   = VK_ACCESS_SHADER_WRITE_BIT;
    light_images.dstAccessMask                   = VK_ACCESS_SHADER_READ_BIT;
    light_images.oldLayout                       = VK_IMAGE_LAYOUT_GENERAL;
    light_images.newLayout                       = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    light_images.srcQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED;
    light_images.dstQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED;
    light_images.image                           = _module.images.light.image.image;
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
    render_area.extent   = _module.swapchain.extent;

    viewport.x        = 0;
    viewport.y        = _module.swapchain.extent.height;
    viewport.width    = _module.swapchain.extent.width;
    viewport.height   = -(int32_t)_module.swapchain.extent.height;
    viewport.minDepth = 0.0;
    viewport.maxDepth = 1.0;

    scissor = render_area;

    vkCmdSetViewport(cmd, 0, 1, &viewport);
    vkCmdSetScissor(cmd, 0, 1, &scissor);

    /* postprocess pass */
    VkRenderPassBeginInfo postprocess_begin_info;
    memset(&postprocess_begin_info, 0, sizeof(VkRenderPassBeginInfo));
    postprocess_begin_info.sType       = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    postprocess_begin_info.renderPass  = _module.renderpasses.postprocess;
    postprocess_begin_info.framebuffer = _module.framebuffers.postprocess.framebuffers[_module.render_context.active_swapchain_image_index];
    postprocess_begin_info.renderArea  = render_area;
    vkCmdBeginRenderPass(cmd, &postprocess_begin_info, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, _module.pipelines.postprocess.layout,
        1, 1, &_module.descriptors.postprocess.descriptor, 0, NULL);
    vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, _module.pipelines.postprocess.pipeline);
    vkCmdDraw(cmd, 3, 1, 0, 0);

    vkCmdEndRenderPass(cmd);

    /* end camera */
    nuvk_camera_end_frame(&_module.scene.camera);

    return NU_SUCCESS;
}
static bool nuvk_renderer_try_render(void)
{
    if (!nuvk_render_context_begin(&_module.render_context, &_module.context, &_module.swapchain)) return false;

    nuvk_renderer_execute_frame();

    if (!nuvk_render_context_end(&_module.render_context, &_module.context, &_module.swapchain)) return false;

    return true;
}
nu_result_t nuvk_renderer_render(void)
{
    if (!_module.render_context_out_of_date) {
        _module.render_context_out_of_date = !nuvk_renderer_try_render();
    } else {
        vkDeviceWaitIdle(_module.context.device);
        nu_vec2u_t size;
        nu_window_get_size(size);
        if (size[0] != 0 && size[1] != 0) {
            nu_result_t result;

            nu_info(NUVK_LOGGER_NAME, "Swapchain updated (%ldx%ld) with %ld frame(s).", size[0], size[1], _module.swapchain.image_count);

            /* recreate swapchain */
            nuvk_swapchain_recreate(&_module.swapchain, &_module.context, size[0], size[1]);

            /* renderpasses */
            vkDestroyRenderPass(_module.context.device, _module.renderpasses.postprocess, &_module.context.allocator);
            result = nuvk_renderpass_postprocess_create(&_module.renderpasses.postprocess, &_module.context, &_module.swapchain);
            NU_CHECK(result == NU_SUCCESS, return result, NUVK_LOGGER_NAME, "Failed to recreate postprocess renderpass.");

            /* framebuffers */
            nuvk_framebuffer_postprocess_destroy(&_module.framebuffers.postprocess, &_module.context);
            result = nuvk_framebuffer_postprocess_create(&_module.framebuffers.postprocess, &_module.context, &_module.swapchain, _module.renderpasses.postprocess);
            NU_CHECK(result == NU_SUCCESS, return NU_FAILURE, NUVK_LOGGER_NAME, "Failed to recreate postprocess framebuffer.");

            /* reset out of date flag */
            _module.render_context_out_of_date = false;
        }
    }

    return NU_SUCCESS;
}

nu_result_t nuvk_renderer_camera_set_fov(nu_renderer_camera_t handle, float fov)
{
    _module.scene.camera.fov = fov;

    return NU_SUCCESS;
}
nu_result_t nuvk_renderer_camera_set_view(nu_renderer_camera_t handle, const nu_vec3f_t eye, const nu_vec3f_t forward, const nu_vec3f_t up)
{
    nu_vec3f_copy(eye, _module.scene.camera.eye);
    nu_vec3f_copy(eye, _module.scene.camera.center);
    nu_vec3f_add(_module.scene.camera.center, forward, _module.scene.camera.center);
    nu_vec3f_copy(up, _module.scene.camera.up);

    return NU_SUCCESS;
}

nu_result_t nuvk_renderer_viewport_set_size(const nu_vec2u_t size)
{
    nu_result_t result;

    vkDeviceWaitIdle(_module.context.device);

    nu_vec2u_copy(size, _module.viewport_size);

    /* images */
    nuvk_image_geometry_destroy(&_module.images.geometry, &_module.context, &_module.memory_manager);
    result = nuvk_image_geometry_create(&_module.images.geometry, &_module.context, &_module.memory_manager, _module.viewport_size);
    NU_CHECK(result == NU_SUCCESS, return result, NUVK_LOGGER_NAME, "Failed to create geometry image.");

    nuvk_image_light_destroy(&_module.images.light, &_module.context, &_module.memory_manager);
    result = nuvk_image_light_create(&_module.images.light, &_module.context, &_module.memory_manager, _module.viewport_size);
    NU_CHECK(result == NU_SUCCESS, return result, NUVK_LOGGER_NAME, "Failed to create light image.");

    /* framebuffers */
    nuvk_framebuffer_geometry_destroy(&_module.framebuffers.geometry, &_module.context);
    result = nuvk_framebuffer_geometry_create(&_module.framebuffers.geometry, &_module.context, &_module.images.geometry, _module.renderpasses.geometry);
    NU_CHECK(result == NU_SUCCESS, return NU_FAILURE, NUVK_LOGGER_NAME, "Failed to create geometry framebuffer.");

    /* descriptors */
    result = nuvk_descriptor_light_update_images(&_module.descriptors.light, &_module.context, &_module.images.geometry, &_module.images.light);
    NU_CHECK(result == NU_SUCCESS, return NU_FAILURE, NUVK_LOGGER_NAME, "Failed to update light descriptor.");

    result = nuvk_descriptor_postprocess_update_images(&_module.descriptors.postprocess, &_module.context, &_module.images.light);
    NU_CHECK(result == NU_SUCCESS, return NU_FAILURE, NUVK_LOGGER_NAME, "Failed to update postprocess descriptor.");
    
    return NU_SUCCESS;
}

nu_result_t nuvk_renderer_sdf_create(const nuvk_sdf_info_t *info, nuvk_sdf_t *handle)
{
    nu_result_t result;
    result = nuvk_assets_sdf_create(&_module.assets, &_module.buffers.instances, info, handle);
    NU_CHECK(result == NU_SUCCESS, return result, NUVK_LOGGER_NAME, "Failed to create sdf asset.");

    nuvk_sdf_data_t *data = (nuvk_sdf_data_t*)(*handle);
    result = nuvk_scene_sdf_pool_create(&_module.scene, &_module.buffers.instances,
        _module.render_context.active_inflight_frame_index, data);
    NU_CHECK(result == NU_SUCCESS, return result, NUVK_LOGGER_NAME, "Failed to create sdf pool.");
    
    /* update generator */
    const nuvk_sdf_data_t **sdfs = (const nuvk_sdf_data_t**)nu_indexed_array_get_data_const(_module.assets.sdfs);
    uint32_t sdf_count = nu_indexed_array_get_size(_module.assets.sdfs);
    result = nuvk_pipeline_generator_update_sdfs(&_module.pipelines.generator, sdfs, sdf_count);
    NU_CHECK(result == NU_SUCCESS, return result, NUVK_LOGGER_NAME, "Failed to update generator.");

    /* recompile pipelines */
    result = nuvk_pipeline_geometry_recompile(&_module.pipelines.geometry, &_module.context, &_module.shader_manager, 
        _module.renderpasses.geometry, &_module.pipelines.generator);
    NU_CHECK(result == NU_SUCCESS, return result, NUVK_LOGGER_NAME, "Failed to recompile geometry pipeline.");
    result = nuvk_pipeline_light_recompile(&_module.pipelines.light, &_module.context, &_module.shader_manager, 
        &_module.descriptors, &_module.pipelines.generator);
    NU_CHECK(result == NU_SUCCESS, return result, NUVK_LOGGER_NAME, "Failed to recompile light pipeline.");

    return result;
}
nu_result_t nuvk_renderer_sdf_destroy(nuvk_sdf_t handle)
{
    return NU_SUCCESS;
}
nu_result_t nuvk_renderer_sdf_get_primitive(nuvk_sdf_primitives_t type, nuvk_sdf_t *handle)
{
    *handle = _module.primitives[type];
    return NU_SUCCESS;
}
nu_result_t nuvk_renderer_material_create(const nuvk_material_info_t *info, nuvk_material_t *handle)
{
    return nuvk_assets_material_create(&_module.assets, &_module.buffers.materials,
        _module.render_context.active_inflight_frame_index, info, handle);
}
nu_result_t nuvk_renderer_material_destroy(nuvk_material_t handle)
{
    return NU_SUCCESS;
}
nu_result_t nuvk_renderer_sdf_instance_create(const nuvk_sdf_instance_info_t *info, nuvk_sdf_instance_t *handle)
{
    return nuvk_scene_sdf_instance_create(&_module.scene, &_module.buffers.instances, 
        _module.render_context.active_inflight_frame_index, info, handle);
}
nu_result_t nuvk_renderer_sdf_instance_destroy(nuvk_sdf_instance_t handle)
{
    return nuvk_scene_sdf_instance_destroy(&_module.scene, handle);
}
nu_result_t nuvk_renderer_sdf_instance_update_transform(nuvk_sdf_instance_t handle, const nuvk_sdf_transform_t *transform)
{
    return nuvk_scene_sdf_instance_update_transform(&_module.scene, &_module.buffers.instances,
        _module.render_context.active_inflight_frame_index, handle, transform);
}
nu_result_t nuvk_renderer_sdf_instance_update_data(nuvk_sdf_instance_t handle, const void *data)
{
    return nuvk_scene_sdf_instance_update_data(&_module.scene, &_module.buffers.instances,
        _module.render_context.active_inflight_frame_index, handle, data);
}