#ifndef NUVK_SDF_SHADERS_H
#define NUVK_SDF_SHADERS_H

#include <nucleus/module/vulkan/sdf/shader/postprocess.h>

typedef struct {
    nuvk_sdf_shader_postprocess_t postprocess;
} nuvk_sdf_shaders_t;

nu_result_t nuvk_sdf_shaders_initialize(
    nuvk_sdf_shaders_t *shaders,
    const nuvk_context_t *context,
    const nuvk_shader_manager_t *shader_manager
);
nu_result_t nuvk_sdf_shaders_terminate(
    nuvk_sdf_shaders_t *shaders,
    const nuvk_context_t *context
);

#endif