#ifndef NUVK_SDF_FRAMEBUFFERS_H
#define NUVK_SDF_FRAMEBUFFERS_H

#include <nucleus/module/vulkan/sdf/framebuffer/geometry.h>
#include <nucleus/module/vulkan/sdf/framebuffer/postprocess.h>
#include <nucleus/module/vulkan/sdf/image/images.h>
#include <nucleus/module/vulkan/sdf/renderpass/renderpasses.h>

typedef struct {
    nuvk_sdf_framebuffer_geometry_t geometry;
    nuvk_sdf_framebuffer_postprocess_t postprocess;
} nuvk_sdf_framebuffers_t;

nu_result_t nuvk_sdf_framebuffers_initialize(
    nuvk_sdf_framebuffers_t *framebuffers,
    const nuvk_context_t *context,
    const nuvk_swapchain_t *swapchain,
    const nuvk_sdf_images_t *images,
    const nuvk_sdf_renderpasses_t *renderpasses
);
nu_result_t nuvk_sdf_framebuffers_terminate(
    nuvk_sdf_framebuffers_t *framebuffers,
    const nuvk_context_t *context
);
nu_result_t nuvk_sdf_framebuffers_update_swapchain(
    nuvk_sdf_framebuffers_t *framebuffers,
    const nuvk_context_t *context,
    const nuvk_swapchain_t *swapchain,
    const nuvk_sdf_images_t *images,
    const nuvk_sdf_renderpasses_t *renderpasses
);

#endif