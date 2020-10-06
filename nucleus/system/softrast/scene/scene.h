#ifndef NUSR_SCENE_H
#define NUSR_SCENE_H

#include "../memory/renderbuffer.h"
#include "../module/interface.h"

typedef struct {
    nu_vec3_t eye;
    nu_vec3_t center;
    nu_vec3_t up;
    float fov;
    float near;
    float far;
} nusr_camera_t;

typedef struct {
    uint32_t mesh;
    uint32_t texture;
    bool has_texture;
    nu_mat4_t transform;
    bool active;
} nusr_staticmesh_t;

nu_result_t nusr_scene_initialize(void);
nu_result_t nusr_scene_terminate(void);
nu_result_t nusr_scene_render(nusr_renderbuffer_t *renderbuffer);

nu_result_t nusr_scene_camera_set_fov(nu_renderer_camera_handle_t handle, float fov);
nu_result_t nusr_scene_camera_set_eye(nu_renderer_camera_handle_t handle, const nu_vec3_t eye);
nu_result_t nusr_scene_camera_set_center(nu_renderer_camera_handle_t handle, const nu_vec3_t center);
nu_result_t nusr_scene_camera_set_transform(const nu_mat4_t transform);

nu_result_t nusr_scene_staticmesh_create(nu_renderer_staticmesh_handle_t *handle, const nu_renderer_staticmesh_create_info_t *info);
nu_result_t nusr_scene_staticmesh_destroy(nu_renderer_staticmesh_handle_t handle);
nu_result_t nusr_scene_staticmesh_set_transform(nu_renderer_staticmesh_handle_t handle, const nu_mat4_t m);

#endif