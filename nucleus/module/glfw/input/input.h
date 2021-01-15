#ifndef NUGLFW_INPUT_H
#define NUGLFW_INPUT_H

#include <nucleus/nucleus.h>

/* window interface */
nu_result_t nuglfw_input_initialize(void);
nu_result_t nuglfw_input_terminate(void);
nu_result_t nuglfw_input_update(void);

nu_result_t nuglfw_input_get_keyboard_state(nu_button_state_t *state, nu_keyboard_t button);
nu_result_t nuglfw_input_get_keyboard_text(const char **text, uint32_t *length);
nu_result_t nuglfw_input_get_mouse_state(nu_button_state_t *state, nu_mouse_t button);
nu_result_t nuglfw_input_get_mouse_motion(nu_vec2f_t motion);
nu_result_t nuglfw_input_get_mouse_scroll(nu_vec2f_t scroll);
nu_result_t nuglfw_input_get_cursor_mode(nu_cursor_mode_t *mode);
nu_result_t nuglfw_input_set_cursor_mode(nu_cursor_mode_t mode);

/* utility function */
nu_result_t nuglfw_input_event_preupdate(void);

#endif