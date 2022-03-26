/* Generated file: DO NOT EDIT ! */
#ifndef NUGLFW_IMPLEMENTATION_H
#define NUGLFW_IMPLEMENTATION_H

#include <nucleus/nucleus.h>

/* window interface */
nu_result_t nuglfw_window_get_required_instance_extensions_impl(const char*** extensions, uint32_t* extension_count);
nu_result_t nuglfw_window_create_window_surface_impl(void* instance, void* surface, const void* allocator_ptr);
nu_result_t nuglfw_window_present_surface_impl(const nu_vec2u_t size, void* pixels);
nu_result_t nuglfw_window_swap_buffers_impl(void);
/* window interface */
nu_result_t nuglfw_window_initialize_impl(void);
nu_result_t nuglfw_window_terminate_impl(void);
nu_result_t nuglfw_window_start_impl(void);
nu_result_t nuglfw_window_stop_impl(void);
nu_result_t nuglfw_window_update_impl(void);
nu_result_t nuglfw_window_set_size_impl(const nu_vec2u_t size);
nu_result_t nuglfw_window_get_size_impl(nu_vec2u_t size);
nu_result_t nuglfw_window_set_title_impl(const char* title);
nu_result_t nuglfw_window_set_mode_impl(nu_window_mode_t mode);
/* input interface */
nu_result_t nuglfw_input_initialize_impl(void);
nu_result_t nuglfw_input_terminate_impl(void);
nu_result_t nuglfw_input_start_impl(void);
nu_result_t nuglfw_input_stop_impl(void);
nu_result_t nuglfw_input_update_impl(void);
nu_result_t nuglfw_input_get_keyboard_state_impl(nu_keyboard_t keyboard, nu_button_state_t* state);
nu_result_t nuglfw_input_get_keyboard_text_impl(const char** text, uint32_t* length);
nu_result_t nuglfw_input_get_mouse_state_impl(nu_mouse_t mouse, nu_button_state_t* state);
nu_result_t nuglfw_input_get_mouse_motion_impl(nu_vec2f_t motion);
nu_result_t nuglfw_input_get_mouse_scroll_impl(nu_vec2f_t scroll);
nu_result_t nuglfw_input_get_cursor_mode_impl(nu_cursor_mode_t* mode);
nu_result_t nuglfw_input_set_cursor_mode_impl(nu_cursor_mode_t mode);

#endif