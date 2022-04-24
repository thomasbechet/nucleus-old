/* Generated file: DO NOT EDIT ! */
#ifndef NUVK_MODULE_INTERFACE_H
#define NUVK_MODULE_INTERFACE_H

#include <nucleus/module/vulkan/module/definition.h>

/* loader */
#ifdef NUVK_LOADER_IMPLEMENTATION
    nuvk_renderer_sdf_create_pfn_t nuvk_renderer_sdf_create = NULL;
    nuvk_renderer_sdf_destroy_pfn_t nuvk_renderer_sdf_destroy = NULL;
    nuvk_renderer_sdf_get_primitive_pfn_t nuvk_renderer_sdf_get_primitive = NULL;
    nuvk_renderer_material_create_pfn_t nuvk_renderer_material_create = NULL;
    nuvk_renderer_material_destroy_pfn_t nuvk_renderer_material_destroy = NULL;
    nuvk_renderer_sdf_instance_create_pfn_t nuvk_renderer_sdf_instance_create = NULL;
    nuvk_renderer_sdf_instance_destroy_pfn_t nuvk_renderer_sdf_instance_destroy = NULL;
    nuvk_renderer_sdf_instance_update_transform_pfn_t nuvk_renderer_sdf_instance_update_transform = NULL;
    nuvk_renderer_sdf_instance_update_data_pfn_t nuvk_renderer_sdf_instance_update_data = NULL;
    nu_result_t nuvk_renderer_interface_load(nu_module_t module)
    {
        nuvk_renderer_interface_t interface;
        nu_result_t result = nu_module_get_interface(module, NUVK_RENDERER_INTERFACE_NAME, &interface);
        if (result == NU_SUCCESS) {
            nuvk_renderer_sdf_create = interface.sdf_create;
            nuvk_renderer_sdf_destroy = interface.sdf_destroy;
            nuvk_renderer_sdf_get_primitive = interface.sdf_get_primitive;
            nuvk_renderer_material_create = interface.material_create;
            nuvk_renderer_material_destroy = interface.material_destroy;
            nuvk_renderer_sdf_instance_create = interface.sdf_instance_create;
            nuvk_renderer_sdf_instance_destroy = interface.sdf_instance_destroy;
            nuvk_renderer_sdf_instance_update_transform = interface.sdf_instance_update_transform;
            nuvk_renderer_sdf_instance_update_data = interface.sdf_instance_update_data;
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
    extern nuvk_renderer_sdf_create_pfn_t nuvk_renderer_sdf_create;
    extern nuvk_renderer_sdf_destroy_pfn_t nuvk_renderer_sdf_destroy;
    extern nuvk_renderer_sdf_get_primitive_pfn_t nuvk_renderer_sdf_get_primitive;
    extern nuvk_renderer_material_create_pfn_t nuvk_renderer_material_create;
    extern nuvk_renderer_material_destroy_pfn_t nuvk_renderer_material_destroy;
    extern nuvk_renderer_sdf_instance_create_pfn_t nuvk_renderer_sdf_instance_create;
    extern nuvk_renderer_sdf_instance_destroy_pfn_t nuvk_renderer_sdf_instance_destroy;
    extern nuvk_renderer_sdf_instance_update_transform_pfn_t nuvk_renderer_sdf_instance_update_transform;
    extern nuvk_renderer_sdf_instance_update_data_pfn_t nuvk_renderer_sdf_instance_update_data;
    nu_result_t nuvk_renderer_interface_load(nu_module_t module);
    nu_result_t nuvk_interface_load_all(nu_module_t module);
#endif

#endif