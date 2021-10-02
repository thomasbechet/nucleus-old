#ifndef NUVK_RENDERER_H
#define NUVK_RENDERER_H

#include <nucleus/module/vulkan/sdf/scene/interface.h>

nu_result_t nuvk_renderer_initialize(void);
nu_result_t nuvk_renderer_terminate(void);
nu_result_t nuvk_renderer_render(void);

nu_result_t nuvk_renderer_camera_set_fov(nu_renderer_camera_t handle, float fov);
nu_result_t nuvk_renderer_camera_set_view(nu_renderer_camera_t handle, const nu_vec3f_t eye, const nu_vec3f_t forward, const nu_vec3f_t up);

nu_result_t nuvk_renderer_viewport_set_size(const nu_vec2u_t size);

nu_result_t nuvk_sdf_instance_type_register(const nuvk_sdf_instance_type_info_t *info, nuvk_sdf_instance_type_t *handle);
nu_result_t nuvk_sdf_instance_type_get(nuvk_sdf_instance_type_primitives_t type, nuvk_sdf_instance_type_t *handle);
nu_result_t nuvk_sdf_instance_create(const nuvk_sdf_instance_info_t *info, nuvk_sdf_instance_t *handle);
nu_result_t nuvk_sdf_instance_destroy(nuvk_sdf_instance_t handle);
nu_result_t nuvk_sdf_instance_update_transform(nuvk_sdf_instance_t handle, const nuvk_sdf_transform_t *transform);
nu_result_t nuvk_sdf_instance_update_data(nuvk_sdf_instance_t handle, const void *data);

#endif