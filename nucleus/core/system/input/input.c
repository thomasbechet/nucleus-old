#include <nucleus/core/system/input/input.h>

#include <nucleus/core/config/interface.h>
#include <nucleus/core/coresystem/memory/interface.h>
#include <nucleus/core/coresystem/logger/interface.h>

#define NU_LOGGER_INPUT_NAME "INPUT"

typedef struct {
    nu_module_t module;
    nu_input_interface_t interface;
} nu_system_data_t;

static nu_system_data_t _system;

nu_result_t nu_input_initialize(void)
{
    nu_result_t result;

    memset(&_system, 0, sizeof(nu_system_data_t));
    nu_input_api_t api = nu_config_get().input.api;

    if (api != NU_INPUT_API_NONE) {
        /* load module */
        if (api == NU_INPUT_API_GLFW) { /* use existing glfw window module */
            _system.module = nu_window_get_module(); /* copy module handle */
        } else {
            _system.module = NU_NULL_HANDLE;
        }
        NU_CHECK(_system.module != NU_NULL_HANDLE, return NU_FAILURE, NU_LOGGER_INPUT_NAME, "Unsupported api.");

        /* get input interface */
        result = nu_module_get_interface(_system.module, NU_INPUT_INTERFACE_NAME, &_system.interface);
        NU_CHECK(result == NU_SUCCESS, return result, NU_LOGGER_INPUT_NAME, "Failed to get interface.");

        /* initialize input system */
        if (_system.interface.initialize) {
            result = _system.interface.initialize();
            NU_CHECK(result == NU_SUCCESS, return result, NU_LOGGER_INPUT_NAME, "Failed to initialize input system.");
        }
    } else {
        nu_info(NU_LOGGER_INPUT_NAME, "Running in passive mode.");
    }
    
    return NU_SUCCESS;
}
nu_result_t nu_input_terminate(void)
{
    if (_system.interface.terminate) {
        return _system.interface.terminate();
    } else {
        return NU_SUCCESS;
    }
}
nu_result_t nu_input_start(void)
{
    if (_system.interface.start) {
        return _system.interface.start();
    } else {
        return NU_SUCCESS;
    }
}
nu_result_t nu_input_stop(void)
{
    if (_system.interface.stop) {
        return _system.interface.stop();
    } else {
        return NU_SUCCESS;
    }
}
nu_result_t nu_input_update(void)
{
    if (_system.interface.update) {
        return _system.interface.update();
    } else {
        return NU_SUCCESS;
    }
}

nu_module_t nu_input_get_module(void)
{
    return _system.module;
}

nu_result_t nu_input_get_keyboard_state(nu_keyboard_t button, nu_button_state_t *state)
{
    if (_system.interface.get_keyboard_state) {
        return _system.interface.get_keyboard_state(button, state);
    } else {
        *state = NU_BUTTON_RELEASED;
        return NU_SUCCESS;
    }
}
nu_result_t nu_input_get_keyboard_text(const char **text, uint32_t *length)
{
    if (_system.interface.get_keyboard_text) {
        return _system.interface.get_keyboard_text(text, length);
    } else {
        *text = NULL;
        *length = 0;
        return NU_FAILURE;
    }
}
nu_result_t nu_input_get_mouse_state(nu_mouse_t button, nu_button_state_t *state)
{
    if (_system.interface.get_mouse_state) {
        return _system.interface.get_mouse_state(button, state);
    } else {
        *state = NU_BUTTON_RELEASED;
        return NU_FAILURE;
    }
}
nu_result_t nu_input_get_mouse_motion(nu_vec2f_t motion)
{
    if (_system.interface.get_mouse_motion) {
        return _system.interface.get_mouse_motion(motion);
    } else {
        nu_vec2f_zero(motion);
        return NU_FAILURE;
    }
}
nu_result_t nu_input_get_mouse_scroll(nu_vec2f_t scroll)
{
    if (_system.interface.get_mouse_scroll) {
        return _system.interface.get_mouse_scroll(scroll);
    } else {
        nu_vec2f_zero(scroll);
        return NU_FAILURE;
    }
}
nu_result_t nu_input_get_cursor_mode(nu_cursor_mode_t *mode)
{
    if (_system.interface.get_cursor_mode) {
        return _system.interface.get_cursor_mode(mode);
    } else {
        *mode = NU_CURSOR_MODE_DISABLE;
        return NU_SUCCESS;
    }
}
nu_result_t nu_input_set_cursor_mode(nu_cursor_mode_t mode)
{
    if (_system.interface.set_cursor_mode) {
        return _system.interface.set_cursor_mode(mode);
    } else {
        return NU_SUCCESS;
    }
}