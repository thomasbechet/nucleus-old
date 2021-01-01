#ifndef NUSR_MATERIAL_H
#define NUSR_MATERIAL_H

#include "../module/interface.h"

typedef struct {
    uint32_t diffuse_texture;
    nu_vec3_t diffuse_uniform;
    bool use_diffuse_uniform;
} nusr_material_t;

nu_result_t nusr_material_initialize(void);
nu_result_t nusr_material_terminate(void);

nu_result_t nusr_material_create(nu_renderer_material_handle_t *handle, const nu_renderer_material_create_info_t *info);
nu_result_t nusr_material_destroy(nu_renderer_material_handle_t handle);
nu_result_t nusr_material_get(uint32_t id, nusr_material_t **p);

#endif