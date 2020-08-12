#include "input.h"

#include "../window/window.h"

static int glfw_keyboard_buttons[] = 
{
    GLFW_KEY_A,
    GLFW_KEY_B,
    GLFW_KEY_C,
    GLFW_KEY_D,
    GLFW_KEY_E,
    GLFW_KEY_F,
    GLFW_KEY_G,
    GLFW_KEY_H,
    GLFW_KEY_I,
    GLFW_KEY_J,
    GLFW_KEY_K,
    GLFW_KEY_L,
    GLFW_KEY_M,
    GLFW_KEY_N,
    GLFW_KEY_O,
    GLFW_KEY_P,
    GLFW_KEY_Q,
    GLFW_KEY_R,
    GLFW_KEY_S,
    GLFW_KEY_T,
    GLFW_KEY_U,
    GLFW_KEY_V,
    GLFW_KEY_W,
    GLFW_KEY_X,
    GLFW_KEY_Y,
    GLFW_KEY_Z,

    GLFW_KEY_F1,
    GLFW_KEY_F2,
    GLFW_KEY_F3,
    GLFW_KEY_F4,
    GLFW_KEY_F5,
    GLFW_KEY_F6,
    GLFW_KEY_F7,
    GLFW_KEY_F8,
    GLFW_KEY_F9,
    GLFW_KEY_F10,
    GLFW_KEY_F11,
    GLFW_KEY_F12,
    
    GLFW_KEY_0,
    GLFW_KEY_1,
    GLFW_KEY_2,
    GLFW_KEY_3,
    GLFW_KEY_4,
    GLFW_KEY_5,
    GLFW_KEY_6,
    GLFW_KEY_7,
    GLFW_KEY_8,
    GLFW_KEY_9,

    GLFW_KEY_ESCAPE,
    GLFW_KEY_SPACE,
    GLFW_KEY_ENTER,
    GLFW_KEY_TAB,
    GLFW_KEY_BACKSPACE,
    GLFW_KEY_LEFT_SHIFT,
    GLFW_KEY_LEFT_CONTROL
};

static int glfw_mouse_buttons[] = 
{
    GLFW_MOUSE_BUTTON_LEFT,
    GLFW_MOUSE_BUTTON_RIGHT,
    GLFW_MOUSE_BUTTON_MIDDLE,
    GLFW_MOUSE_BUTTON_1,
    GLFW_MOUSE_BUTTON_2,
    GLFW_MOUSE_BUTTON_3,
    GLFW_MOUSE_BUTTON_4,
    GLFW_MOUSE_BUTTON_5,
    GLFW_MOUSE_BUTTON_6
};

typedef struct {
    nu_vec2_t mouse_motion;
    nu_vec2_t mouse_position;
    nu_vec2_t mouse_old_position;
} nuglfw_input_data_t;

static nuglfw_input_data_t _data;

static void nuglfw_cursor_position_callback(GLFWwindow *window, double xpos, double ypos)
{
    _data.mouse_position[0] = (float)xpos;
    _data.mouse_position[1] = (float)ypos;
}

nu_result_t nuglfw_input_initialize(void)
{
    glfwSetInputMode(nuglfw_get_window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if(glfwRawMouseMotionSupported())
        glfwSetInputMode(nuglfw_get_window(), GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

    glfwSetCursorPosCallback(nuglfw_get_window(), nuglfw_cursor_position_callback);

    double xpos, ypos;
    glfwGetCursorPos(nuglfw_get_window(), &xpos, &ypos);
    _data.mouse_position[0] = (float)xpos;
    _data.mouse_position[1] = (float)ypos;
    nu_vec2_copy(_data.mouse_position, _data.mouse_old_position);

    return NU_SUCCESS;
}
nu_result_t nuglfw_input_terminate(void)
{
    return NU_SUCCESS;
}
nu_result_t nuglfw_input_update(void)
{
    nu_vec2_sub(_data.mouse_position, _data.mouse_old_position, _data.mouse_motion);
    nu_vec2_copy(_data.mouse_position, _data.mouse_old_position);

    return NU_SUCCESS;
}
nu_result_t nuglfw_input_get_keyboard_state(nu_button_state_t *state, nu_keyboard_t button)
{
    *state = (glfwGetKey(nuglfw_get_window(), glfw_keyboard_buttons[(size_t)button]) == GLFW_PRESS) ? NU_BUTTON_PRESSED : NU_BUTTON_RELEASED;
    return NU_SUCCESS;
}
nu_result_t nuglfw_input_get_mouse_state(nu_button_state_t *state, nu_mouse_t button)
{
    *state = (glfwGetMouseButton(nuglfw_get_window(), glfw_mouse_buttons[(size_t)button]) == GLFW_PRESS) ? NU_BUTTON_PRESSED : NU_BUTTON_RELEASED;
    return NU_SUCCESS;
}
nu_result_t nuglfw_input_get_mouse_motion(nu_vec2_t motion)
{
    nu_vec2_copy(_data.mouse_motion, motion);
    return NU_SUCCESS;
}