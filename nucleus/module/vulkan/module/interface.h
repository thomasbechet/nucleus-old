/* Generated file: DO NOT EDIT ! */
#ifndef NUVK_MODULE_INTERFACE_H
#define NUVK_MODULE_INTERFACE_H

#include <nucleus/module/vulkan/module/definition.h>

/* loader */
#ifdef NUVK_LOADER_IMPLEMENTATION
    nuvk_renderer_interface_t _nuvk_renderer_interface;
    nu_result_t nuvk_renderer_interface_load(nu_module_t module)
    {
        return nu_module_get_interface(module, NUVK_RENDERER_INTERFACE_NAME, &_nuvk_renderer_interface);
    }
    nu_result_t nuvk_renderer_sdf_create(const nuvk_sdf_info_t* info, nuvk_sdf_t* handle)
    {
        return _nuvk_renderer_interface.sdf_create(info, handle);
    }
    nu_result_t nuvk_renderer_sdf_destroy(nuvk_sdf_t handle)
    {
        return _nuvk_renderer_interface.sdf_destroy(handle);
    }
    nu_result_t nuvk_renderer_sdf_get_primitive(nuvk_sdf_primitives_t primitive, nuvk_sdf_t* sdf)
    {
        return _nuvk_renderer_interface.sdf_get_primitive(primitive, sdf);
    }
    nu_result_t nuvk_renderer_material_create(const nuvk_material_info_t* info, nuvk_material_t* handle)
    {
        return _nuvk_renderer_interface.material_create(info, handle);
    }
    nu_result_t nuvk_renderer_material_destroy(nuvk_material_t handle)
    {
        return _nuvk_renderer_interface.material_destroy(handle);
    }
    nu_result_t nuvk_renderer_sdf_instance_create(const nuvk_sdf_instance_info_t* info, nuvk_sdf_instance_t* handle)
    {
        return _nuvk_renderer_interface.sdf_instance_create(info, handle);
    }
    nu_result_t nuvk_renderer_sdf_instance_destroy(nuvk_sdf_instance_t handle)
    {
        return _nuvk_renderer_interface.sdf_instance_destroy(handle);
    }
    nu_result_t nuvk_renderer_sdf_instance_update_transform(nuvk_sdf_instance_t handle, const nuvk_sdf_transform_t* transform)
    {
        return _nuvk_renderer_interface.sdf_instance_update_transform(handle, transform);
    }
    nu_result_t nuvk_renderer_sdf_instance_update_data(nuvk_sdf_instance_t handle, const void* data)
    {
        return _nuvk_renderer_interface.sdf_instance_update_data(handle, data);
    }
#else
    extern nuvk_renderer_interface_t _nuvk_renderer_interface;
    nu_result_t nuvk_renderer_interface_load(nu_module_t module);
#endif

#endif