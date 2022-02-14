/* Generated file: DO NOT EDIT ! */
#include <nucleus/module/glfw/module/module.h>

#include <nucleus/module/glfw/module/definition.h>
#include <nucleus/module/glfw/input/input.h>
#include <nucleus/module/glfw/window/window.h>

static const uint32_t interface_count = 4;
static const char *interfaces[] = {
    NU_PLUGIN_INTERFACE_NAME, 
    NUGLFW_WINDOW_INTERFACE_NAME, 
    NU_WINDOW_INTERFACE_NAME, 
    NU_INPUT_INTERFACE_NAME
};

static const uint32_t plugin_count = 0;
static const char *plugins[] = {
};

static nu_result_t plugin_get_list(uint32_t *count, const char ***plugin_list)
{
    *count = plugin_count;
    *plugin_list = plugins;
    return NU_SUCCESS;
}
static nu_result_t plugin_get_callbacks(const char *name, nu_plugin_callbacks_t *callbacks)
{

    return NU_FAILURE;
}

nu_result_t nu_module_info(nu_module_info_t *info)
{
    info->name = NUGLFW_MODULE_NAME;
    info->id = NUGLFW_MODULE_ID;
    info->flags = NU_MODULE_FLAG_NONE;
    info->interface_count = interface_count;
    info->interfaces = interfaces;

    return NU_SUCCESS;
}
nu_result_t nu_module_interface(const char *name, void *interface)
{
    if (NU_MATCH(name, NU_PLUGIN_INTERFACE_NAME)) {
        nu_plugin_interface_t *i = (nu_plugin_interface_t*)interface;

        i->get_callbacks = plugin_get_callbacks;
        i->get_list = plugin_get_list;

        return NU_SUCCESS;
    } else if (NU_MATCH(name, NUGLFW_WINDOW_INTERFACE_NAME)) {
        nuglfw_window_interface_t *i = (nuglfw_window_interface_t*)interface;
        
        i->get_required_instance_extensions = nuglfw_window_get_required_instance_extensions;
        i->create_window_surface = nuglfw_window_create_window_surface;
        i->present_surface = nuglfw_window_present_surface;
        i->swap_buffers = nuglfw_window_swap_buffers;

        return NU_SUCCESS;
    } else if (NU_MATCH(name, NU_WINDOW_INTERFACE_NAME)) {
        nu_window_interface_t *i = (nu_window_interface_t*)interface;
        
        i->initialize = nuglfw_window_initialize;
        i->terminate = nuglfw_window_terminate;
        i->start = nuglfw_window_start;
        i->stop = nuglfw_window_stop;
        i->update = nuglfw_window_update;
        i->set_size = nuglfw_window_set_size;
        i->get_size = nuglfw_window_get_size;
        i->set_title = nuglfw_window_set_title;
        i->set_mode = nuglfw_window_set_mode;

        return NU_SUCCESS;
    } else if (NU_MATCH(name, NU_INPUT_INTERFACE_NAME)) {
        nu_input_interface_t *i = (nu_input_interface_t*)interface;
        
        i->initialize = nuglfw_input_initialize;
        i->terminate = nuglfw_input_terminate;
        i->start = nuglfw_input_start;
        i->stop = nuglfw_input_stop;
        i->update = nuglfw_input_update;
        i->get_keyboard_state = nuglfw_input_get_keyboard_state;
        i->get_keyboard_text = nuglfw_input_get_keyboard_text;
        i->get_mouse_state = nuglfw_input_get_mouse_state;
        i->get_mouse_motion = nuglfw_input_get_mouse_motion;
        i->get_mouse_scroll = nuglfw_input_get_mouse_scroll;
        i->get_cursor_mode = nuglfw_input_get_cursor_mode;
        i->set_cursor_mode = nuglfw_input_set_cursor_mode;

        return NU_SUCCESS;
    }

    return NU_FAILURE;
}