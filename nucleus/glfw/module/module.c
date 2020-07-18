#include "module.h"

#include "../window/window.h"
#include "../input/input.h"

static const uint32_t interface_count = 3;
static const char *interfaces[] = {
    NU_WINDOW_INTERFACE_NAME,
    NU_INPUT_INTERFACE_NAME,
    NUGLFW_WINDOW_INTERFACE_NAME
};

nu_result_t nu_module_get_info(nu_module_info_t *info)
{
    info->id = NUGLFW_MODULE_ID;
    info->flags = NU_MODULE_FLAG_TYPE_WINDOW | NU_MODULE_FLAG_TYPE_INPUT;
    info->interface_count = interface_count;
    info->interfaces = interfaces;

    return NU_SUCCESS;
}

nu_result_t nu_window_interface_loader(nu_window_interface_t *interface)
{
    interface->initialize = nuglfw_window_initialize;
    interface->terminate  = nuglfw_window_terminate;
    interface->update     = nuglfw_window_update;
    interface->get_size   = nuglfw_window_get_size;

    return NU_SUCCESS;
}
nu_result_t nu_input_interface_loader(nu_input_interface_t *interface)
{
    interface->initialize         = nuglfw_input_initialize;
    interface->terminate          = nuglfw_input_terminate;
    interface->update             = nuglfw_input_update;
    interface->get_keyboard_state = nuglfw_input_get_keyboard_state;
    interface->get_mouse_state    = nuglfw_input_get_mouse_state;
    interface->get_mouse_motion   = nuglfw_input_get_mouse_motion;

    return NU_SUCCESS;
}
nu_result_t nuglfw_window_interface_loader(nuglfw_window_interface_t *interface)
{
    interface->create_window_surface            = nuglfw_create_window_surface;
    interface->get_required_instance_extensions = nuglfw_get_required_instance_extensions;
    interface->present_surface                  = nuglfw_present_surface;

    return NU_SUCCESS;
}