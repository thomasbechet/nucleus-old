/* Generated file: DO NOT EDIT ! */
#ifndef NUGLFW_MODULE_INTERFACE_H
#define NUGLFW_MODULE_INTERFACE_H

#include <nucleus/module/glfw/module/definition.h>

/* loader */
#ifdef NUGLFW_LOADER_IMPLEMENTATION
    nuglfw_window_interface_t _nuglfw_window_interface;
    nu_result_t nuglfw_window_interface_load(nu_module_t module)
    {
        return nu_module_get_interface(module, NUGLFW_WINDOW_INTERFACE_NAME, &_nuglfw_window_interface);
    }
    nu_result_t nuglfw_window_get_required_instance_extensions(const char*** extensions, uint32_t* extension_count)
    {
        return _nuglfw_window_interface.get_required_instance_extensions(extensions, extension_count);
    }
    nu_result_t nuglfw_window_create_window_surface(void* instance, void* surface, const void* allocator_ptr)
    {
        return _nuglfw_window_interface.create_window_surface(instance, surface, allocator_ptr);
    }
    nu_result_t nuglfw_window_present_surface(const nu_vec2u_t size, void* pixels)
    {
        return _nuglfw_window_interface.present_surface(size, pixels);
    }
    nu_result_t nuglfw_window_swap_buffers(void)
    {
        return _nuglfw_window_interface.swap_buffers();
    }
#else
    extern nuglfw_window_interface_t _nuglfw_window_interface;
    nu_result_t nuglfw_window_interface_load(nu_module_t module);
#endif

#endif