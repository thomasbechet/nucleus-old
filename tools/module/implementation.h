/* Generated file: DO NOT EDIT ! */
#ifndef NUVK_IMPLEMENTATION_H
#define NUVK_IMPLEMENTATION_H

#include <nucleus/nucleus.h>
#include <nucleus/module/vulkan/assets/interface.h>
#include <nucleus/module/vulkan/scene/interface.h>

/* renderer interface */
nu_result_t nuvk_renderer_initialize_impl(void);
nu_result_t nuvk_renderer_terminate_impl(void);
nu_result_t nuvk_renderer_start_impl(void);
nu_result_t nuvk_renderer_stop_impl(void);
nu_result_t nuvk_renderer_render_impl(void);
nu_result_t nuvk_renderer_camera_create_impl(const nu_renderer_camera_create_info_t* info, nu_renderer_camera_t* handle);
nu_result_t nuvk_renderer_camera_destroy_impl(nu_renderer_camera_t handle);
nu_result_t nuvk_renderer_camera_set_fov_impl(nu_renderer_camera_t handle, float fov);
nu_result_t nuvk_renderer_camera_set_view_impl(nu_renderer_camera_t handle, const nu_vec3f_t eye, const nu_vec3f_t forward, const nu_vec3f_t up);
nu_result_t nuvk_renderer_viewport_set_size_impl(const nu_vec2u_t size);
nu_result_t nuvk_renderer_viewport_get_size_impl(nu_vec2u_t size);
/* renderer interface */
nu_result_t nuvk_renderer_sdf_create_impl(const nuvk_sdf_info_t* info, nuvk_sdf_t* handle);
nu_result_t nuvk_renderer_sdf_destroy_impl(nuvk_sdf_t handle);
nu_result_t nuvk_renderer_sdf_get_primitive_impl(nuvk_sdf_primitives_t primitive, nuvk_sdf_t* sdf);
nu_result_t nuvk_renderer_material_create_impl(const nuvk_material_info_t* info, nuvk_material_t* handle);
nu_result_t nuvk_renderer_material_destroy_impl(nuvk_material_t handle);
nu_result_t nuvk_renderer_sdf_instance_create_impl(const nuvk_sdf_instance_info_t* info, nuvk_sdf_instance_t* handle);
nu_result_t nuvk_renderer_sdf_instance_destroy_impl(nuvk_sdf_instance_t handle);
nu_result_t nuvk_renderer_sdf_instance_update_transform_impl(nuvk_sdf_instance_t handle, const nuvk_sdf_transform_t* transform);
nu_result_t nuvk_renderer_sdf_instance_update_data_impl(nuvk_sdf_instance_t handle, const void* data);

#endif