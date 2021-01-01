#ifndef NUSR_TEXTURE_H
#define NUSR_TEXTURE_H

#include "../module/interface.h"

typedef struct {
    uint32_t width;
    uint32_t height;
    uint32_t *data;
} nusr_texture_t;

nu_result_t nusr_texture_initialize(void);
nu_result_t nusr_texture_terminate(void);

nu_result_t nusr_texture_create(nu_renderer_texture_handle_t *handle, const nu_renderer_texture_create_info_t *info);
nu_result_t nusr_texture_destroy(nu_renderer_texture_handle_t handle);
nu_result_t nusr_texture_get(uint32_t id, nusr_texture_t **p);

#endif