#ifndef NUVK_SDF_BUFFERS_H
#define NUVK_SDF_BUFFERS_H

#include <nucleus/module/vulkan/sdf/buffer/environment.h>
#include <nucleus/module/vulkan/sdf/buffer/instances.h>

typedef struct {
    nuvk_sdf_buffer_environment_t environment;
    nuvk_sdf_buffer_instances_t instances;
} nuvk_sdf_buffers_t;

#endif