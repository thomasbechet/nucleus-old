/* Generated file: DO NOT EDIT ! */
#ifndef NUVK_MODULE_INTERFACE_H
#define NUVK_MODULE_INTERFACE_H

#include <nucleus/module/vulkan/module/definition.h>

/* functions definition */
typedef nu_result_t (*__nuvk_renderer_sdf_create_pfn_t)(const nuvk_sdf_info_t* info, nuvk_sdf_t* handle);
typedef nu_result_t (*__nuvk_renderer_sdf_destroy_pfn_t)(nuvk_sdf_t handle);
typedef nu_result_t (*__nuvk_renderer_sdf_get_primitive_pfn_t)(nuvk_sdf_primitives_t primitive, nuvk_sdf_t* sdf);
typedef nu_result_t (*__nuvk_renderer_material_create_pfn_t)(const nuvk_material_info_t* info, nuvk_material_t* handle);
typedef nu_result_t (*__nuvk_renderer_material_destroy_pfn_t)(nuvk_material_t handle);
typedef nu_result_t (*__nuvk_renderer_sdf_instance_create_pfn_t)(const nuvk_sdf_instance_info_t* info, nuvk_sdf_instance_t* handle);
typedef nu_result_t (*__nuvk_renderer_sdf_instance_destroy_pfn_t)(nuvk_sdf_instance_t handle);
typedef nu_result_t (*__nuvk_renderer_sdf_instance_update_transform_pfn_t)(nuvk_sdf_instance_t handle, const nuvk_sdf_transform_t* transform);
typedef nu_result_t (*__nuvk_renderer_sdf_instance_update_data_pfn_t)(nuvk_sdf_instance_t handle, const void* data);

/* loader */
#ifdef NUVK_LOADER_IMPLEMENTATION
    __nuvk_renderer_sdf_create_pfn_t __nuvk_renderer_sdf_create = NULL;
    __nuvk_renderer_sdf_destroy_pfn_t __nuvk_renderer_sdf_destroy = NULL;
    __nuvk_renderer_sdf_get_primitive_pfn_t __nuvk_renderer_sdf_get_primitive = NULL;
    __nuvk_renderer_material_create_pfn_t __nuvk_renderer_material_create = NULL;
    __nuvk_renderer_material_destroy_pfn_t __nuvk_renderer_material_destroy = NULL;
    __nuvk_renderer_sdf_instance_create_pfn_t __nuvk_renderer_sdf_instance_create = NULL;
    __nuvk_renderer_sdf_instance_destroy_pfn_t __nuvk_renderer_sdf_instance_destroy = NULL;
    __nuvk_renderer_sdf_instance_update_transform_pfn_t __nuvk_renderer_sdf_instance_update_transform = NULL;
    __nuvk_renderer_sdf_instance_update_data_pfn_t __nuvk_renderer_sdf_instance_update_data = NULL;
    nu_result_t nuvk_renderer_interface_load(nu_module_t module)
    {
        nuvk_renderer_interface_t interface;
        nu_result_t result = nu_module_get_interface(module, NUVK_RENDERER_INTERFACE_NAME, &interface);
        if (result == NU_SUCCESS) {
            __nuvk_renderer_sdf_create = interface.sdf_create;
            __nuvk_renderer_sdf_destroy = interface.sdf_destroy;
            __nuvk_renderer_sdf_get_primitive = interface.sdf_get_primitive;
            __nuvk_renderer_material_create = interface.material_create;
            __nuvk_renderer_material_destroy = interface.material_destroy;
            __nuvk_renderer_sdf_instance_create = interface.sdf_instance_create;
            __nuvk_renderer_sdf_instance_destroy = interface.sdf_instance_destroy;
            __nuvk_renderer_sdf_instance_update_transform = interface.sdf_instance_update_transform;
            __nuvk_renderer_sdf_instance_update_data = interface.sdf_instance_update_data;
        }
        return result;
    }
    nu_result_t nuvk_interface_load_all(nu_module_t module)
    {
        nu_result_t result = NU_SUCCESS;
        result &= nuvk_renderer_interface_load(module);
        return result;
    }
#else
    extern __nuvk_renderer_sdf_create_pfn_t __nuvk_renderer_sdf_create;
    extern __nuvk_renderer_sdf_destroy_pfn_t __nuvk_renderer_sdf_destroy;
    extern __nuvk_renderer_sdf_get_primitive_pfn_t __nuvk_renderer_sdf_get_primitive;
    extern __nuvk_renderer_material_create_pfn_t __nuvk_renderer_material_create;
    extern __nuvk_renderer_material_destroy_pfn_t __nuvk_renderer_material_destroy;
    extern __nuvk_renderer_sdf_instance_create_pfn_t __nuvk_renderer_sdf_instance_create;
    extern __nuvk_renderer_sdf_instance_destroy_pfn_t __nuvk_renderer_sdf_instance_destroy;
    extern __nuvk_renderer_sdf_instance_update_transform_pfn_t __nuvk_renderer_sdf_instance_update_transform;
    extern __nuvk_renderer_sdf_instance_update_data_pfn_t __nuvk_renderer_sdf_instance_update_data;
    nu_result_t nuvk_renderer_interface_load(nu_module_t module);
    nu_result_t nuvk_interface_load_all(nu_module_t module);
#endif
#define nuvk_renderer_sdf_create NU_IDENTITY(__nuvk_renderer_sdf_create)
#define nuvk_renderer_sdf_destroy NU_IDENTITY(__nuvk_renderer_sdf_destroy)
#define nuvk_renderer_sdf_get_primitive NU_IDENTITY(__nuvk_renderer_sdf_get_primitive)
#define nuvk_renderer_material_create NU_IDENTITY(__nuvk_renderer_material_create)
#define nuvk_renderer_material_destroy NU_IDENTITY(__nuvk_renderer_material_destroy)
#define nuvk_renderer_sdf_instance_create NU_IDENTITY(__nuvk_renderer_sdf_instance_create)
#define nuvk_renderer_sdf_instance_destroy NU_IDENTITY(__nuvk_renderer_sdf_instance_destroy)
#define nuvk_renderer_sdf_instance_update_transform NU_IDENTITY(__nuvk_renderer_sdf_instance_update_transform)
#define nuvk_renderer_sdf_instance_update_data NU_IDENTITY(__nuvk_renderer_sdf_instance_update_data)

#endif