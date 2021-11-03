#ifndef NUVK_ASSETS_SDF_H
#define NUVK_ASSETS_SDF_H

#include <nucleus/module/vulkan/assets/interface.h>

typedef struct {
    uint32_t data_size;
    const char *glsl_data_code;
    const char *glsl_sdf_code;
    nuvk_sdf_pfn_t c_sdf_code;
    nuvk_sdf_aabb_pfn_t c_aabb_code;
    uint32_t max_instance_count;
    uint32_t pool_id;
    uint32_t buffer_id;
    uint32_t id;
} nuvk_sdf_data_t;


#endif