#ifndef NUVK_SDF_FRAMEBUFFERS_H
#define NUVK_SDF_FRAMEBUFFERS_H

#include <nucleus/module/vulkan/sdf/framebuffer/geometry.h>
#include <nucleus/module/vulkan/sdf/framebuffer/postprocess.h>

typedef struct {
    nuvk_sdf_framebuffer_geometry_t geometry;
    nuvk_sdf_framebuffer_postprocess_t postprocess;
} nuvk_sdf_framebuffers_t;

#endif