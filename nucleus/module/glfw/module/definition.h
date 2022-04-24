/* Generated file: DO NOT EDIT ! */
#ifndef NUGLFW_MODULE_DEFINITION_H
#define NUGLFW_MODULE_DEFINITION_H

#include <nucleus/nucleus.h>

/* module */
#define NUGLFW_MODULE_NAME "nucleus-glfw"
#define NUGLFW_MODULE_ID 1

/* interface */
#define NUGLFW_WINDOW_INTERFACE_NAME "nuglfw_window_interface"
typedef nu_result_t (*nuglfw_window_get_required_instance_extensions_pfn_t)(const char*** extensions, uint32_t* extension_count);
typedef nu_result_t (*nuglfw_window_create_window_surface_pfn_t)(void* instance, void* surface, const void* allocator_ptr);
typedef nu_result_t (*nuglfw_window_present_surface_pfn_t)(const nu_vec2u_t size, void* pixels);
typedef nu_result_t (*nuglfw_window_swap_buffers_pfn_t)(void);

typedef struct {
    nuglfw_window_get_required_instance_extensions_pfn_t get_required_instance_extensions;
    nuglfw_window_create_window_surface_pfn_t create_window_surface;
    nuglfw_window_present_surface_pfn_t present_surface;
    nuglfw_window_swap_buffers_pfn_t swap_buffers;
} nuglfw_window_interface_t;


/* plugin */

#endif