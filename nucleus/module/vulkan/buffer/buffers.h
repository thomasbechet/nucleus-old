#ifndef NUVK_BUFFERS_H
#define NUVK_BUFFERS_H

#include <nucleus/module/vulkan/buffer/environment.h>
#include <nucleus/module/vulkan/buffer/instances.h>
#include <nucleus/module/vulkan/buffer/materials.h>

typedef struct {
    nuvk_buffer_environment_t environment;
    nuvk_buffer_instances_t instances;
    nuvk_buffer_materials_t materials;
} nuvk_buffers_t;

#endif