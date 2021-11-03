#ifndef NUVK_RENDERPASSES_H
#define NUVK_RENDERPASSES_H

#include <nucleus/module/vulkan/renderpass/geometry.h>
#include <nucleus/module/vulkan/renderpass/postprocess.h>

typedef struct {
    VkRenderPass geometry;
    VkRenderPass postprocess;
} nuvk_renderpasses_t;

#endif