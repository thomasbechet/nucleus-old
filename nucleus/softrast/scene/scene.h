#ifndef NUSR_SCENE_H
#define NUSR_SCENE_H

#include "../memory/framebuffer.h"
#include "../module/interface.h"
#include "../../core/nucleus_core.h"

nu_result_t nusr_scene_initialize(void);
nu_result_t nusr_scene_terminate(void);
nu_result_t nusr_scene_render(nusr_framebuffer_t *framebuffer);

nu_result_t nusr_scene_camera_set_fov(float fov);
nu_result_t nusr_scene_camera_set_eye(const vec3 eye);
nu_result_t nusr_scene_camera_set_center(const vec3 center);
nu_result_t nusr_scene_camera_set_transform(const mat4 transform);

nu_result_t nusr_scene_staticmesh_create(uint32_t *id, const nusr_staticmesh_create_info_t *info);
nu_result_t nusr_scene_staticmesh_destroy(uint32_t id);
nu_result_t nusr_scene_staticmesh_set_transform(uint32_t id, const mat4 m);

#endif