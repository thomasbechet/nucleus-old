#ifndef NUGLFW_WINDOW_H
#define NUGLFW_WINDOW_H

#include <nucleus/nucleus.h>

/* window interface */
nu_result_t nuglfw_window_initialize(void);
nu_result_t nuglfw_window_terminate(void);
nu_result_t nuglfw_window_start(void);
nu_result_t nuglfw_window_stop(void);
nu_result_t nuglfw_window_update(void);

nu_result_t nuglfw_window_set_size(const nu_vec2u_t size);
nu_result_t nuglfw_window_get_size(nu_vec2u_t size);
nu_result_t nuglfw_window_set_title(const char *title);
nu_result_t nuglfw_window_set_mode(nu_window_mode_t mode);

/* glfw window interface */
nu_result_t nuglfw_window_get_required_instance_extensions(const char*** extensions, uint32_t* extension_count);
nu_result_t nuglfw_window_create_window_surface(void* instance, void* surface, const void* allocator_ptr);
nu_result_t nuglfw_window_present_surface(const nu_vec2u_t size, void* pixels);
nu_result_t nuglfw_window_swap_buffers(void);
nu_result_t nuglfw_swap_buffers(void);

/* utility functions */
void *nuglfw_get_window(void);

#endif