#ifndef NUVK_CAMERA_H
#define NUVK_CAMERA_H

#include <nucleus/module/vulkan/core/swapchain.h>

typedef struct {
    float fov;
    float ratio;
    float focal_length;
    nu_vec2u_t resolution;

    nu_vec3f_t eye;
    nu_vec3f_t center;
    nu_vec3f_t up;
    nu_mat4f_t vp_matrix;

    nu_vec3f_t old_eye;
    nu_vec3f_t old_center;
    nu_vec3f_t old_up;
    nu_mat4f_t old_vp_matrix;
} nuvk_camera_t;

nu_result_t nuvk_camera_initialize(nuvk_camera_t *camera);
nu_result_t nuvk_camera_start_frame(
    nuvk_camera_t *camera,
    const nu_vec2u_t viewport_resolution
);
nu_result_t nuvk_camera_end_frame(nuvk_camera_t *camera);

#endif