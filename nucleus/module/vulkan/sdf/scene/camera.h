#ifndef NUVK_SDF_CAMERA_H
#define NUVK_SDF_CAMERA_H

#include <nucleus/module/vulkan/core/swapchain.h>

typedef struct {
    float fov;
    float ratio;

    nu_vec3f_t eye;
    nu_vec3f_t center;
    nu_vec3f_t up;
    nu_mat4f_t vp_matrix;

    nu_vec3f_t old_eye;
    nu_vec3f_t old_center;
    nu_vec3f_t old_up;
    nu_mat4f_t old_vp_matrix;
} nuvk_sdf_camera_t;

nu_result_t nuvk_sdf_camera_initialize(nuvk_sdf_camera_t *camera);
nu_result_t nuvk_sdf_camera_start_frame(
    nuvk_sdf_camera_t *camera,
    const nuvk_swapchain_t *swapchain
);
nu_result_t nuvk_sdf_camera_end_frame(nuvk_sdf_camera_t *camera);

#endif