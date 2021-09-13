#ifndef NUVK_SDF_RENDERPASSES_H
#define NUVK_SDF_RENDERPASSES_H

#include <nucleus/module/vulkan/sdf/renderpass/geometry.h>
#include <nucleus/module/vulkan/sdf/renderpass/postprocess.h>

typedef struct {
    VkRenderPass geometry;
    VkRenderPass postprocess;
} nuvk_sdf_renderpasses_t;

#endif