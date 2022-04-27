/* Generated file: DO NOT EDIT ! */
#ifndef NUGLFW_MODULE_INTERFACE_H
#define NUGLFW_MODULE_INTERFACE_H

#include <nucleus/module/glfw/module/definition.h>

/* loader */
#ifdef NUGLFW_LOADER_IMPLEMENTATION
    nuglfw_window_get_required_instance_extensions_pfn_t nuglfw_window_get_required_instance_extensions = NULL;
    nuglfw_window_create_window_surface_pfn_t nuglfw_window_create_window_surface = NULL;
    nuglfw_window_present_surface_pfn_t nuglfw_window_present_surface = NULL;
    nuglfw_window_swap_buffers_pfn_t nuglfw_window_swap_buffers = NULL;
    nu_result_t nuglfw_window_interface_load(nu_module_t module)
    {
        nuglfw_window_interface_t interface;
        nu_result_t result = nu_module_get_interface(module, NUGLFW_WINDOW_INTERFACE_NAME, &interface);
        if (result == NU_SUCCESS) {
            nuglfw_window_get_required_instance_extensions = interface.get_required_instance_extensions;
            nuglfw_window_create_window_surface = interface.create_window_surface;
            nuglfw_window_present_surface = interface.present_surface;
            nuglfw_window_swap_buffers = interface.swap_buffers;
        }
        return result;
    }
    nu_result_t nuglfw_interface_load_all(nu_module_t module)
    {
        nu_result_t result = NU_SUCCESS;
        result &= nuglfw_window_interface_load(module);
        return result;
    }
#else
    extern nuglfw_window_get_required_instance_extensions_pfn_t nuglfw_window_get_required_instance_extensions;
    extern nuglfw_window_create_window_surface_pfn_t nuglfw_window_create_window_surface;
    extern nuglfw_window_present_surface_pfn_t nuglfw_window_present_surface;
    extern nuglfw_window_swap_buffers_pfn_t nuglfw_window_swap_buffers;
    nu_result_t nuglfw_window_interface_load(nu_module_t module);
    nu_result_t nuglfw_interface_load_all(nu_module_t module);
#endif

#endif