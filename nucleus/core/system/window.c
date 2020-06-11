#include "window.h"

#include "../../glfw/module/interface.h"

#define NU_WINDOW_LOG_NAME "[WINDOW] "

static const char *nu_window_api_names[] = {
    "nucleus-window-none",
    NUGLFW_MODULE_NAME
};

typedef struct {
    nu_module_t module;
    nu_window_interface_t interface;
} nu_system_window_t;

static nu_system_window_t _system;

nu_result_t nu_system_window_load(nu_window_api_t api)
{
    nu_result_t result;
    result = NU_SUCCESS;

    /* load module */
    result = nu_module_load(&_system.module, nu_window_api_names[api]);
    if (result != NU_SUCCESS) {
        return result;
    }

    /* load interface */
    result = nu_module_load_interface(&_system.module, NU_WINDOW_INTERFACE, (nu_pfn_t*)&_system.interface);
    if (result != NU_SUCCESS) {
        nu_warning(NU_WINDOW_LOG_NAME"Missing window functions from module '%s'.\n", nu_window_api_names[api]);
    }

    /* initialize window system */
    result = _system.interface.initialize();
    if (result != NU_SUCCESS) {
        nu_warning(NU_WINDOW_LOG_NAME"Failed to initialize window system.\n");
        return result;
    }

    return result;
}
nu_result_t nu_system_window_unload(void)
{
    /* terminate window system */
    _system.interface.terminate();

    /* unload module */
    nu_module_unload(&_system.module);
    memset(&_system.interface, 0, sizeof(nu_window_interface_t));
    
    return NU_SUCCESS;
}
nu_result_t nu_system_window_update(void)
{
    _system.interface.update();
    return NU_SUCCESS;
}

const nu_module_t *nu_system_window_get_module(void)
{
    return &_system.module;
}

nu_result_t nu_window_get_size(uint32_t *width, uint32_t *height)
{
    return _system.interface.get_size(width, height);
}
nu_result_t nu_window_set_size(const uint32_t *size)
{
    return NU_SUCCESS;
}
nu_result_t nu_window_set_title(const char *title)
{
    return NU_SUCCESS;
}