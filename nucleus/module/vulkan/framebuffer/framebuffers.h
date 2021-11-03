#ifndef NUVK_FRAMEBUFFERS_H
#define NUVK_FRAMEBUFFERS_H

#include <nucleus/module/vulkan/framebuffer/geometry.h>
#include <nucleus/module/vulkan/framebuffer/postprocess.h>

typedef struct {
    nuvk_framebuffer_geometry_t geometry;
    nuvk_framebuffer_postprocess_t postprocess;
} nuvk_framebuffers_t;

#endif