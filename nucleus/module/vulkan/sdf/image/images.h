#ifndef NUVK_SDF_IMAGES_H
#define NUVK_SDF_IMAGES_H

#include <nucleus/module/vulkan/sdf/image/geometry.h>
#include <nucleus/module/vulkan/sdf/image/light.h>

typedef struct {
    nuvk_sdf_image_geometry_t geometry;
    nuvk_sdf_image_light_t light;
} nuvk_sdf_images_t;

#endif