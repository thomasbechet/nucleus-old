#include <nucleus/core/system/input/input.h>

#include <nucleus/core/config/interface.h>
#include <nucleus/core/coresystem/memory/interface.h>
#include <nucleus/core/coresystem/logger/interface.h>

#define NU_LOGGER_INPUT_NAME "[INPUT] "

typedef struct {
    nu_module_handle_t module;
    nu_input_interface_t interface;
} nu_input_data_t;

static nu_input_data_t _data;

nu_result_t nu_input_initialize(void)
{
    nu_result_t result;
    result = NU_SUCCESS;

    nu_input_api_t api = nu_config_get().input.api;

    /* load module */
    if (api == NU_INPUT_API_GLFW) { /* use existing glfw window module */
        _data.module = nu_window_get_module_handle(); /* copy module handle */
    } else {
        nu_warning(NU_LOGGER_INPUT_NAME"Unsupported api.\n");
        return NU_FAILURE;
    }

    /* load input interface */
    result = nu_module_load_interface(_data.module, NU_INPUT_INTERFACE_NAME, &_data.interface);
    if (result != NU_SUCCESS) {
        nu_warning(NU_LOGGER_INPUT_NAME"Failed to load interface.\n");
        return result;
    }

    /* initialize input system */
    result = _data.interface.initialize();
    if (result != NU_SUCCESS) {
        nu_warning(NU_LOGGER_INPUT_NAME"Failed to initialize input system.\n");
        return result;
    }

    return NU_SUCCESS;
}
nu_result_t nu_input_terminate(void)
{
    return _data.interface.terminate();
}
nu_result_t nu_input_start(void)
{
    return _data.interface.start();
}
nu_result_t nu_input_stop(void)
{
    return _data.interface.stop();
}
nu_result_t nu_input_update(void)
{
    return _data.interface.update();
}

nu_module_handle_t nu_input_get_module_handle(void)
{
    return _data.module;
}

nu_result_t nu_input_get_keyboard_state(nu_keyboard_t button, nu_button_state_t *state)
{
    return _data.interface.get_keyboard_state(button, state);
}
nu_result_t nu_input_get_keyboard_text(const char **text, uint32_t *length)
{
    return _data.interface.get_keyboard_text(text, length);
}
nu_result_t nu_input_get_mouse_state(nu_mouse_t button, nu_button_state_t *state)
{
    return _data.interface.get_mouse_state(button, state);
}
nu_result_t nu_input_get_mouse_motion(nu_vec2f_t motion)
{
    return _data.interface.get_mouse_motion(motion);
}
nu_result_t nu_input_get_mouse_scroll(nu_vec2f_t scroll)
{
    return _data.interface.get_mouse_scroll(scroll);
}
nu_result_t nu_input_get_cursor_mode(nu_cursor_mode_t *mode)
{
    return _data.interface.get_cursor_mode(mode);
}
nu_result_t nu_input_set_cursor_mode(nu_cursor_mode_t mode)
{
    return _data.interface.set_cursor_mode(mode);
}