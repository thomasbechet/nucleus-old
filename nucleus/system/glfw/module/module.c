#include "module.h"

#include "../window/window.h"
#include "../input/input.h"

static const uint32_t plugin_count = 0;
static const char *plugins[] = {};

nu_result_t nu_module_get_info(nu_module_info_t *info)
{
    info->name         = NUGLFW_MODULE_NAME;
    info->id           = NUGLFW_MODULE_ID;
    info->flags        = NU_MODULE_FLAG_TYPE_WINDOW | NU_MODULE_FLAG_TYPE_INPUT;
    info->plugin_count = plugin_count;
    info->plugins      = plugins;

    return NU_SUCCESS;
}

nu_result_t nu_window_get_interface(nu_window_interface_t *interface)
{
    interface->initialize = nuglfw_window_initialize;
    interface->terminate  = nuglfw_window_terminate;
    interface->update     = nuglfw_window_update;

    interface->set_size   = nuglfw_window_set_size;
    interface->get_size   = nuglfw_window_get_size;
    interface->set_title  = nuglfw_window_set_title;
    interface->set_mode   = nuglfw_window_set_mode;

    return NU_SUCCESS;
}
nu_result_t nu_input_get_interface(nu_input_interface_t *interface)
{
    interface->initialize         = nuglfw_input_initialize;
    interface->terminate          = nuglfw_input_terminate;
    interface->update             = nuglfw_input_update;
    interface->get_keyboard_state = nuglfw_input_get_keyboard_state;
    interface->get_keyboard_text  = nuglfw_input_get_keyboard_text;
    interface->get_mouse_state    = nuglfw_input_get_mouse_state;
    interface->get_mouse_motion   = nuglfw_input_get_mouse_motion;
    interface->get_cursor_mode    = nuglfw_input_get_cursor_mode;
    interface->set_cursor_mode    = nuglfw_input_set_cursor_mode;

    return NU_SUCCESS;
}
nu_result_t nuglfw_window_get_interface(nuglfw_window_interface_t *interface)
{
    interface->create_window_surface            = nuglfw_create_window_surface;
    interface->get_required_instance_extensions = nuglfw_get_required_instance_extensions;
    interface->present_surface                  = nuglfw_present_surface;
    interface->swap_buffers                     = nuglfw_swap_buffers;

    return NU_SUCCESS;
}