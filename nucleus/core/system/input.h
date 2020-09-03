#ifndef NU_SYSTEM_INPUT_H
#define NU_SYSTEM_INPUT_H

#include "../module/module.h"
#include "../module/interface.h"

typedef enum {
    NU_INPUT_API_NONE = 0,
    NU_INPUT_API_GLFW = 1
} nu_input_api_t;

/* private system functions */
nu_result_t nu_system_input_initialize(void);
nu_result_t nu_system_input_terminate(void);
nu_result_t nu_system_input_update(void);

/* public system functions */
NU_API nu_module_handle_t nu_system_input_get_module_handle(void);

/* public input functions */
NU_API nu_result_t nu_input_get_keyboard_state(nu_button_state_t *state, nu_keyboard_t button);
NU_API nu_result_t nu_input_get_keyboard_text(const char **text, uint32_t *length);
NU_API nu_result_t nu_input_get_mouse_state(nu_button_state_t *state, nu_mouse_t button);
NU_API nu_result_t nu_input_get_mouse_motion(nu_vec2_t motion);
NU_API nu_result_t nu_input_get_cursor_mode(nu_cursor_mode_t *mode);
NU_API nu_result_t nu_input_set_cursor_mode(nu_cursor_mode_t mode);

#endif