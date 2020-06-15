#include "window.h"

#include "../context.h"

static const uint32_t interface_count = 2;
static const char *interfaces[] = {
    NU_WINDOW_INTERFACE_NAME,
    NUGLFW_WINDOW_INTERFACE_NAME
};

nu_result_t nu_module_get_info(nu_module_info_t *info)
{
    info->id = NUGLFW_MODULE_ID;
    info->type = NU_MODULE_TYPE_WINDOW;
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
nu_result_t nuglfw_window_interface_loader(nuglfw_window_interface_t *interface)
{
    interface->create_window_surface            = nuglfw_create_window_surface;
    interface->get_required_instance_extensions = nuglfw_get_required_instance_extensions;

    return NU_SUCCESS;
}