/* Generated file: DO NOT EDIT ! */
#ifndef NUVK_MODULE_DEFINITION_H
#define NUVK_MODULE_DEFINITION_H

#include <nucleus/nucleus.h>
#include <nucleus/module/vulkan/assets/interface.h>
#include <nucleus/module/vulkan/scene/interface.h>

/* module */
#define NUVK_MODULE_NAME "nucleus-vulkan"
#define NUVK_MODULE_ID 8

/* interface */
#define NUVK_RENDERER_INTERFACE_NAME "nuvk_renderer_interface"
typedef nu_result_t (*nuvk_renderer_sdf_create_pfn_t)(const nuvk_sdf_info_t* info, nuvk_sdf_t* handle);
typedef nu_result_t (*nuvk_renderer_sdf_destroy_pfn_t)(nuvk_sdf_t handle);
typedef nu_result_t (*nuvk_renderer_sdf_get_primitive_pfn_t)(nuvk_sdf_primitives_t primitive, nuvk_sdf_t* sdf);
typedef nu_result_t (*nuvk_renderer_material_create_pfn_t)(const nuvk_material_info_t* info, nuvk_material_t* handle);
typedef nu_result_t (*nuvk_renderer_material_destroy_pfn_t)(nuvk_material_t handle);
typedef nu_result_t (*nuvk_renderer_sdf_instance_create_pfn_t)(const nuvk_sdf_instance_info_t* info, nuvk_sdf_instance_t* handle);
typedef nu_result_t (*nuvk_renderer_sdf_instance_destroy_pfn_t)(nuvk_sdf_instance_t handle);
typedef nu_result_t (*nuvk_renderer_sdf_instance_update_transform_pfn_t)(nuvk_sdf_instance_t handle, const nuvk_sdf_transform_t* transform);
typedef nu_result_t (*nuvk_renderer_sdf_instance_update_data_pfn_t)(nuvk_sdf_instance_t handle, const void* data);

typedef struct {
    nuvk_renderer_sdf_create_pfn_t sdf_create;
    nuvk_renderer_sdf_destroy_pfn_t sdf_destroy;
    nuvk_renderer_sdf_get_primitive_pfn_t sdf_get_primitive;
    nuvk_renderer_material_create_pfn_t material_create;
    nuvk_renderer_material_destroy_pfn_t material_destroy;
    nuvk_renderer_sdf_instance_create_pfn_t sdf_instance_create;
    nuvk_renderer_sdf_instance_destroy_pfn_t sdf_instance_destroy;
    nuvk_renderer_sdf_instance_update_transform_pfn_t sdf_instance_update_transform;
    nuvk_renderer_sdf_instance_update_data_pfn_t sdf_instance_update_data;
} nuvk_renderer_interface_t;


/* plugin */

#endif