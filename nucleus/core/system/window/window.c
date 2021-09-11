#include <nucleus/core/system/window/window.h>

#include <nucleus/core/config/interface.h>
#include <nucleus/module/glfw.h>

#define NU_LOGGER_WINDOW_NAME "WINDOW"

static const char *nu_window_api_names[] = {
    "$MODULE_DIR/nucleus-window-none",
    "$MODULE_DIR/"NUGLFW_MODULE_NAME
};

typedef struct {
    nu_module_t module;
    nu_window_interface_t interface;
} nu_system_data_t;

static nu_system_data_t _system;

nu_result_t nu_window_initialize(void)
{
    nu_result_t result;

    memset(&_system, 0, sizeof(nu_system_data_t));
    nu_window_api_t api = nu_config_get().window.api;

    if (api != NU_WINDOW_API_NONE) {
        /* get module */
        result = nu_module_load(nu_window_api_names[api], &_system.module);
        if (result != NU_SUCCESS) {
            return result;
        }

        /* get window interface */
        result = nu_module_get_interface(_system.module, NU_WINDOW_INTERFACE_NAME, &_system.interface);
        if (result != NU_SUCCESS) {
            nu_error(NU_LOGGER_WINDOW_NAME, "Failed to get interface.");
            return result;
        }

        /* initialize window system */
        if (_system.interface.initialize) {
            result = _system.interface.initialize();
            if (result != NU_SUCCESS) {
                nu_error(NU_LOGGER_WINDOW_NAME, "Failed to initialize window system.");
                return result;
            }
        }
    } else {
        nu_info(NU_LOGGER_WINDOW_NAME, "Running in passive mode.");
    }

    return NU_SUCCESS;
}
nu_result_t nu_window_terminate(void)
{
    if (_system.interface.terminate) {
        return _system.interface.terminate();
    } else {
        return NU_SUCCESS;
    }
}
nu_result_t nu_window_start(void)
{
    if (_system.interface.start) {
        return _system.interface.start();
    } else {
        return NU_SUCCESS;
    }
}
nu_result_t nu_window_stop(void)
{
    if (_system.interface.stop) {
        return _system.interface.stop();
    } else {
        return NU_SUCCESS;
    }
}
nu_result_t nu_window_update(void)
{
    if (_system.interface.update) {
        return _system.interface.update();
    } else {
        return NU_SUCCESS;
    }
}

nu_module_t nu_window_get_module(void)
{
    return _system.module;
}

nu_result_t nu_window_set_size(const nu_vec2u_t size)
{
    if (_system.interface.set_size) {
        return _system.interface.set_size(size);
    } else {
        return NU_SUCCESS;
    }
}
nu_result_t nu_window_get_size(nu_vec2u_t size)
{
    if (_system.interface.get_size) {
        return _system.interface.get_size(size);
    } else {
        size[0] = size[1] = 0;
        return NU_SUCCESS;
    }
}
nu_result_t nu_window_set_title(const char *title)
{
    if (_system.interface.set_title) {
        return _system.interface.set_title(title);
    } else {
        return NU_SUCCESS;
    }
}
nu_result_t nu_window_set_mode(nu_window_mode_t mode)
{
    if (_system.interface.set_mode) {
        return _system.interface.set_mode(mode);
    } else {
        return NU_SUCCESS;
    }
}