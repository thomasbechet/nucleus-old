/* Generated file: DO NOT EDIT ! */
#ifndef NUGLFW_MODULE_INTERFACE_H
#define NUGLFW_MODULE_INTERFACE_H

#include <nucleus/module/glfw/module/definition.h>

/* functions definition */
typedef nu_result_t (*__nuglfw_window_get_required_instance_extensions_pfn_t)(const char*** extensions, uint32_t* extension_count);
typedef nu_result_t (*__nuglfw_window_create_window_surface_pfn_t)(void* instance, void* surface, const void* allocator_ptr);
typedef nu_result_t (*__nuglfw_window_present_surface_pfn_t)(const nu_vec2u_t size, void* pixels);
typedef nu_result_t (*__nuglfw_window_swap_buffers_pfn_t)(void);

/* loader */
#ifdef NUGLFW_LOADER_IMPLEMENTATION
    __nuglfw_window_get_required_instance_extensions_pfn_t __nuglfw_window_get_required_instance_extensions = NULL;
    __nuglfw_window_create_window_surface_pfn_t __nuglfw_window_create_window_surface = NULL;
    __nuglfw_window_present_surface_pfn_t __nuglfw_window_present_surface = NULL;
    __nuglfw_window_swap_buffers_pfn_t __nuglfw_window_swap_buffers = NULL;
    nu_result_t nuglfw_window_interface_load(nu_module_t module)
    {
        nuglfw_window_interface_t interface;
        nu_result_t result = nu_module_get_interface(module, NUGLFW_WINDOW_INTERFACE_NAME, &interface);
        if (result == NU_SUCCESS) {
            __nuglfw_window_get_required_instance_extensions = interface.get_required_instance_extensions;
            __nuglfw_window_create_window_surface = interface.create_window_surface;
            __nuglfw_window_present_surface = interface.present_surface;
            __nuglfw_window_swap_buffers = interface.swap_buffers;
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
    extern __nuglfw_window_get_required_instance_extensions_pfn_t __nuglfw_window_get_required_instance_extensions;
    extern __nuglfw_window_create_window_surface_pfn_t __nuglfw_window_create_window_surface;
    extern __nuglfw_window_present_surface_pfn_t __nuglfw_window_present_surface;
    extern __nuglfw_window_swap_buffers_pfn_t __nuglfw_window_swap_buffers;
    nu_result_t nuglfw_window_interface_load(nu_module_t module);
    nu_result_t nuglfw_interface_load_all(nu_module_t module);
#endif
#define nuglfw_window_get_required_instance_extensions NU_IDENTITY(__nuglfw_window_get_required_instance_extensions)
#define nuglfw_window_create_window_surface NU_IDENTITY(__nuglfw_window_create_window_surface)
#define nuglfw_window_present_surface NU_IDENTITY(__nuglfw_window_present_surface)
#define nuglfw_window_swap_buffers NU_IDENTITY(__nuglfw_window_swap_buffers)

#endif