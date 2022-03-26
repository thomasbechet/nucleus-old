#include <nucleus/module/glfw/module/implementation.h>

#include <nucleus/module/glfw/window/window.h>
#include <nucleus/module/glfw/input/input.h>

/* window interface */
nu_result_t nuglfw_window_get_required_instance_extensions_impl(const char*** extensions, uint32_t* extension_count)
{
    return nuglfw_window_get_required_instance_extensions(extensions, extension_count);
}
nu_result_t nuglfw_window_create_window_surface_impl(void* instance, void* surface, const void* allocator_ptr)
{
    return nuglfw_window_create_window_surface(instance, surface, allocator_ptr);
}
nu_result_t nuglfw_window_present_surface_impl(const nu_vec2u_t size, void* pixels)
{
    return nuglfw_window_present_surface(size, pixels);
}
nu_result_t nuglfw_window_swap_buffers_impl(void)
{
    return nuglfw_window_swap_buffers();
}
/* window interface */
nu_result_t nuglfw_window_initialize_impl(void)
{
    return nuglfw_window_initialize();
}
nu_result_t nuglfw_window_terminate_impl(void)
{
    return nuglfw_window_terminate();
}
nu_result_t nuglfw_window_start_impl(void)
{
    return nuglfw_window_start();
}
nu_result_t nuglfw_window_stop_impl(void)
{
    return nuglfw_window_stop();
}
nu_result_t nuglfw_window_update_impl(void)
{
    return nuglfw_window_update();
}
nu_result_t nuglfw_window_set_size_impl(const nu_vec2u_t size)
{
    return nuglfw_window_set_size(size);
}
nu_result_t nuglfw_window_get_size_impl(nu_vec2u_t size)
{
    return nuglfw_window_get_size(size);
}
nu_result_t nuglfw_window_set_title_impl(const char* title)
{
    return nuglfw_window_set_title(title);
}
nu_result_t nuglfw_window_set_mode_impl(nu_window_mode_t mode)
{
    return nuglfw_window_set_mode(mode);
}
/* input interface */
nu_result_t nuglfw_input_initialize_impl(void)
{
    return nuglfw_input_initialize();
}
nu_result_t nuglfw_input_terminate_impl(void)
{
    return nuglfw_input_terminate();
}
nu_result_t nuglfw_input_start_impl(void)
{
    return nuglfw_input_start();
}
nu_result_t nuglfw_input_stop_impl(void)
{
    return nuglfw_input_stop();
}
nu_result_t nuglfw_input_update_impl(void)
{
    return nuglfw_input_update();
}
nu_result_t nuglfw_input_get_keyboard_state_impl(nu_keyboard_t keyboard, nu_button_state_t* state)
{
    return nuglfw_input_get_keyboard_state(keyboard, state);
}
nu_result_t nuglfw_input_get_keyboard_text_impl(const char** text, uint32_t* length)
{
    return nuglfw_input_get_keyboard_text(text, length);
}
nu_result_t nuglfw_input_get_mouse_state_impl(nu_mouse_t mouse, nu_button_state_t* state)
{
    return nuglfw_input_get_mouse_state(mouse, state);
}
nu_result_t nuglfw_input_get_mouse_motion_impl(nu_vec2f_t motion)
{
    return nuglfw_input_get_mouse_motion(motion);
}
nu_result_t nuglfw_input_get_mouse_scroll_impl(nu_vec2f_t scroll)
{
    return nuglfw_input_get_mouse_scroll(scroll);
}
nu_result_t nuglfw_input_get_cursor_mode_impl(nu_cursor_mode_t* mode)
{
    return nuglfw_input_get_cursor_mode(mode);
}
nu_result_t nuglfw_input_set_cursor_mode_impl(nu_cursor_mode_t mode)
{
    return nuglfw_input_set_cursor_mode(mode);
}