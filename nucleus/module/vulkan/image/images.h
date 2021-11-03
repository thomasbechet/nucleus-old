#ifndef NUVK_IMAGES_H
#define NUVK_IMAGES_H

#include <nucleus/module/vulkan/image/geometry.h>
#include <nucleus/module/vulkan/image/light.h>

typedef struct {
    nuvk_image_geometry_t geometry;
    nuvk_image_light_t light;
} nuvk_images_t;

#endif