#ifndef NUVK_RENDERER_H
#define NUVK_RENDERER_H

#include <nucleus/nucleus.h>

nu_result_t nuvk_renderer_initialize(void);
nu_result_t nuvk_renderer_terminate(void);
nu_result_t nuvk_renderer_render(void);
nu_result_t nuvk_renderer_camera_set_fov(nu_renderer_camera_t handle, float fov);
nu_result_t nuvk_renderer_camera_set_view(nu_renderer_camera_t handle, const nu_vec3f_t eye, const nu_vec3f_t forward, const nu_vec3f_t up);

#endif