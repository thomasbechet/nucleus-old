#ifndef NUSR_SCENE_H
#define NUSR_SCENE_H

#include "../memory/framebuffer.h"
#include "../module/interface.h"
#include "../../core/nucleus_core.h"

nu_result_t nusr_scene_initialize(void);
nu_result_t nusr_scene_terminate(void);
nu_result_t nusr_scene_render(nusr_framebuffer_t *color_buffer, nusr_framebuffer_t *depth_buffer);

nu_result_t nusr_scene_camera_set_fov(float fov);
nu_result_t nusr_scene_camera_set_eye(const nu_vec3_t eye);
nu_result_t nusr_scene_camera_set_center(const nu_vec3_t center);
nu_result_t nusr_scene_camera_set_transform(const nu_mat4_t transform);

nu_result_t nusr_scene_staticmesh_create(uint32_t *id, const nusr_staticmesh_create_info_t *info);
nu_result_t nusr_scene_staticmesh_destroy(uint32_t id);
nu_result_t nusr_scene_staticmesh_set_transform(uint32_t id, const nu_mat4_t m);

#endif