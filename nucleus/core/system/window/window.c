#include "window.h"

#include "../../context/config.h"

#include <nucleus/module/glfw.h>

#define NU_LOGGER_WINDOW_NAME "[WINDOW] "

static const char *nu_window_api_names[] = {
    "engine/module/nucleus-window-none",
    "engine/module/"NUGLFW_MODULE_NAME
};

typedef struct {
    nu_module_handle_t module;
    nu_window_interface_t interface;
} nu_system_window_t;

static nu_system_window_t _system;

nu_result_t nu_system_window_initialize(void)
{
    nu_result_t result;
    result = NU_SUCCESS;

    nu_window_api_t api = nu_config_get().window.api;

    /* get module */
    result = nu_module_load(&_system.module, nu_window_api_names[api]);
    if (result != NU_SUCCESS) {
        return result;
    }

    /* load window interface accessor */
    nu_window_interface_loader_pfn_t load_interface;
    result = nu_module_load_function(_system.module, NU_WINDOW_INTERFACE_LOADER_NAME, (nu_pfn_t*)&load_interface);
    if (result != NU_SUCCESS) {
        nu_warning(NU_LOGGER_WINDOW_NAME"Failed to load window loader.\n");
        return result;
    }

    /* load window interface */
    result = load_interface(&_system.interface);
    if (result != NU_SUCCESS) {
        nu_warning(NU_LOGGER_WINDOW_NAME"Failed to load interface.\n");
        return result;
    }

    /* initialize window system */
    result = _system.interface.initialize();
    if (result != NU_SUCCESS) {
        nu_warning(NU_LOGGER_WINDOW_NAME"Failed to initialize window system.\n");
        return result;
    }

    return result;
}
nu_result_t nu_system_window_terminate(void)
{
    /* terminate window system */
    _system.interface.terminate();
    
    return NU_SUCCESS;
}
nu_result_t nu_system_window_update(void)
{
    _system.interface.update();
    return NU_SUCCESS;
}

nu_module_handle_t nu_system_window_get_module_handle(void)
{
    return _system.module;
}

nu_result_t nu_window_set_size(uint32_t width, uint32_t height)
{
    return _system.interface.set_size(width, height);
}
nu_result_t nu_window_get_size(uint32_t *width, uint32_t *height)
{
    return _system.interface.get_size(width, height);
}
nu_result_t nu_window_set_title(const char *title)
{
    return _system.interface.set_title(title);
}
nu_result_t nu_window_set_mode(nu_window_mode_t mode)
{
    return _system.interface.set_mode(mode);
}