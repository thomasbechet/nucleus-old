#include <nucleus/core/system/window/window.h>

#include <nucleus/core/config/interface.h>

#include <nucleus/module/glfw.h>

#define NU_LOGGER_WINDOW_NAME "[WINDOW] "

static const char *nu_window_api_names[] = {
    "engine/module/nucleus-window-none",
    "engine/module/"NUGLFW_MODULE_NAME
};

typedef struct {
    nu_module_handle_t module;
    nu_window_interface_t interface;
} nu_window_data_t;

static nu_window_data_t _data;

nu_result_t nu_window_initialize(void)
{
    nu_result_t result;
    result = NU_SUCCESS;

    nu_window_api_t api = nu_config_get().window.api;

    /* get module */
    result = nu_module_load(&_data.module, nu_window_api_names[api]);
    if (result != NU_SUCCESS) {
        return result;
    }

    /* load window interface */
    result = nu_module_load_interface(_data.module, NU_WINDOW_INTERFACE_NAME, &_data.interface);
    if (result != NU_SUCCESS) {
        nu_warning(NU_LOGGER_WINDOW_NAME"Failed to load interface.\n");
        return result;
    }

    /* initialize window system */
    result = _data.interface.initialize();
    if (result != NU_SUCCESS) {
        nu_warning(NU_LOGGER_WINDOW_NAME"Failed to initialize window system.\n");
        return result;
    }

    return result;
}
nu_result_t nu_window_terminate(void)
{
    return _data.interface.terminate();
}
nu_result_t nu_window_start(void)
{
    return _data.interface.start();
}
nu_result_t nu_window_stop(void)
{
    return _data.interface.stop();
}
nu_result_t nu_window_update(void)
{
    return _data.interface.update();
}

nu_module_handle_t nu_window_get_module_handle(void)
{
    return _data.module;
}

nu_result_t nu_window_set_size(const nu_vec2u_t size)
{
    return _data.interface.set_size(size);
}
nu_result_t nu_window_get_size(nu_vec2u_t size)
{
    return _data.interface.get_size(size);
}
nu_result_t nu_window_set_title(const char *title)
{
    return _data.interface.set_title(title);
}
nu_result_t nu_window_set_mode(nu_window_mode_t mode)
{
    return _data.interface.set_mode(mode);
}