#ifndef NUVK_SDF_RENDERER_H
#define NUVK_SDF_RENDERER_H

#include <nucleus/module/vulkan/core/render_context.h>
#include <nucleus/module/vulkan/sdf/buffer/buffers.h>
#include <nucleus/module/vulkan/sdf/renderpass/renderpasses.h>
#include <nucleus/module/vulkan/sdf/descriptor/descriptors.h>
#include <nucleus/module/vulkan/sdf/shader/shaders.h>
#include <nucleus/module/vulkan/sdf/pipeline/pipelines.h>
#include <nucleus/module/vulkan/sdf/image/images.h>
#include <nucleus/module/vulkan/sdf/framebuffer/framebuffers.h>
#include <nucleus/module/vulkan/sdf/entity/camera.h>

typedef struct {
    nuvk_sdf_buffers_t buffers;
    nuvk_sdf_renderpasses_t renderpasses;
    nuvk_sdf_descriptors_t descriptors;
    nuvk_sdf_shaders_t shaders;
    nuvk_sdf_pipelines_t pipelines;
    nuvk_sdf_images_t images;
    nuvk_sdf_framebuffers_t framebuffers;
    nuvk_sdf_camera_t camera;
} nuvk_sdf_renderer_t;

nu_result_t nuvk_sdf_renderer_initialize(
    nuvk_sdf_renderer_t *renderer,
    const nuvk_context_t *context,
    const nuvk_memory_manager_t *memory_manager,
    const nuvk_shader_manager_t *shader_manager,
    const nuvk_swapchain_t *swapchain,
    const nuvk_render_context_t *render_context
);
nu_result_t nuvk_sdf_renderer_terminate(
    nuvk_sdf_renderer_t *renderer,
    const nuvk_context_t *context,
    const nuvk_memory_manager_t *memory_manager
);
nu_result_t nuvk_sdf_renderer_render(
    nuvk_sdf_renderer_t *renderer,
    const nuvk_swapchain_t *swapchain,
    const nuvk_render_context_t *render_context
);
nu_result_t nuvk_sdf_renderer_update_swapchain(
    nuvk_sdf_renderer_t *renderer,
    const nuvk_context_t *context,
    const nuvk_memory_manager_t *memory_manager,
    const nuvk_swapchain_t *swapchain
);

#endif