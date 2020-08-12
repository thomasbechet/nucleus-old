#ifndef NUGLFW_INPUT_H
#define NUGLFW_INPUT_H

#include "../../core/nucleus_core.h"

/* window interface */
nu_result_t nuglfw_input_initialize(void);
nu_result_t nuglfw_input_terminate(void);
nu_result_t nuglfw_input_update(void);
nu_result_t nuglfw_input_get_keyboard_state(nu_button_state_t *state, nu_keyboard_t button);
nu_result_t nuglfw_input_get_mouse_state(nu_button_state_t *state, nu_mouse_t button);
nu_result_t nuglfw_input_get_mouse_motion(nu_vec2_t motion);

#endif