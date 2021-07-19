#include <nucleus/core/system/window/window.h>

#include <nucleus/core/config/interface.h>
#include <nucleus/module/glfw.h>

#define NU_LOGGER_WINDOW_NAME "[WINDOW] "

static const char *nu_window_api_names[] = {
    "engine/module/nucleus-window-none",
    "engine/module/"NUGLFW_MODULE_NAME
};

typedef struct {
    nu_module_t module;
    nu_window_interface_t interface;
} nu_system_data_t;

static nu_system_data_t _system;

nu_result_t nu_window_initialize(void)
{
    nu_result_t result;
    result = NU_SUCCESS;

    nu_window_api_t api = nu_config_get().window.api;

    /* get module */
    result = nu_module_load(nu_window_api_names[api], &_system.module);
    if (result != NU_SUCCESS) {
        return result;
    }

    /* load window interface */
    result = nu_module_load_interface(_system.module, NU_WINDOW_INTERFACE_NAME, &_system.interface);
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
nu_result_t nu_window_terminate(void)
{
    return _system.interface.terminate();
}
nu_result_t nu_window_start(void)
{
    return _system.interface.start();
}
nu_result_t nu_window_stop(void)
{
    return _system.interface.stop();
}
nu_result_t nu_window_update(void)
{
    return _system.interface.update();
}

nu_module_t nu_window_get_module(void)
{
    return _system.module;
}

nu_result_t nu_window_set_size(const nu_vec2u_t size)
{
    return _system.interface.set_size(size);
}
nu_result_t nu_window_get_size(nu_vec2u_t size)
{
    return _system.interface.get_size(size);
}
nu_result_t nu_window_set_title(const char *title)
{
    return _system.interface.set_title(title);
}
nu_result_t nu_window_set_mode(nu_window_mode_t mode)
{
    return _system.interface.set_mode(mode);
}