#include <nucleus/module/utils/spectator/spectator.h>

#define NORMAL_SPEED 20.0f / 1000.0f
#define SLOW_SPEED 2.0f / 1000.0f
#define FAST_SPEED 100.0f / 1000.0f
#define ROLL_SPEED 90.0f / 1000.0f
#define ROTATION_SENSIBILITY 0.1f
#define ZOOM_SPEED 5.0f

typedef struct {
    nu_renderer_camera_t camera;
    nu_vec3f_t velocity;
    nu_transform_t transform;
    float yaw;
    float pitch;
    float fov;
    bool free_mode;
} nuutils_module_data_t;

static nuutils_module_data_t _module;

nu_result_t nuutils_spectator_plugin_initialize(void)
{
    nu_transform_identity(&_module.transform);
    nu_vec3f_copy((nu_vec3f_t){0.0f, 0.0f, 0.5f}, _module.transform.translation);
    _module.free_mode = false;
    _module.yaw       = 0.0f;
    _module.pitch     = 0.0f;
    _module.fov       = 80.0f;
    return NU_SUCCESS;
}
nu_result_t nuutils_spectator_plugin_terminate(void)
{
    return NU_SUCCESS;
}
nu_result_t nuutils_spectator_plugin_update(void)
{
    /* check cursor mode */
    nu_cursor_mode_t cursor_mode;
    nu_input_get_cursor_mode(&cursor_mode);
    if (!(cursor_mode & NU_CURSOR_MODE_DISABLE)) return NU_SUCCESS;

    /* get deltatime */
    float delta = nu_context_get_delta_time();

    /* get inputs */
    nu_button_state_t slow_mode_state, fast_mode_state, change_mode_state;
    nu_button_state_t forward_state, backward_state, left_state, right_state, up_state, down_state;
    nu_button_state_t roll_left_state, roll_right_state;
    nu_input_get_keyboard_state(NU_KEYBOARD_LCONTROL, &slow_mode_state);
    nu_input_get_keyboard_state(NU_KEYBOARD_LSHIT, &fast_mode_state);
    nu_input_get_keyboard_state(NU_KEYBOARD_C, &change_mode_state);
    nu_input_get_keyboard_state(NU_KEYBOARD_W, &forward_state);
    nu_input_get_keyboard_state(NU_KEYBOARD_S, &backward_state);
    nu_input_get_keyboard_state(NU_KEYBOARD_A, &left_state);
    nu_input_get_keyboard_state(NU_KEYBOARD_D, &right_state);
    nu_input_get_keyboard_state(NU_KEYBOARD_X, &up_state);
    nu_input_get_keyboard_state(NU_KEYBOARD_Z, &down_state);
    nu_input_get_keyboard_state(NU_KEYBOARD_Q, &roll_left_state);
    nu_input_get_keyboard_state(NU_KEYBOARD_E, &roll_right_state);

    /* update mode */
    if (change_mode_state & NU_BUTTON_JUST_PRESSED) _module.free_mode = !_module.free_mode;

    /* translation */
    nu_vec3f_t direction, temp;
    nu_vec3f_zero(direction);
    if (forward_state & NU_BUTTON_PRESSED) {
        nu_transform_get_forward(&_module.transform, temp);
        nu_vec3f_add(direction, temp, direction);
    }
    if (backward_state & NU_BUTTON_PRESSED) {
        nu_transform_get_backward(&_module.transform, temp);
        nu_vec3f_add(direction, temp, direction);
    }
    if (left_state & NU_BUTTON_PRESSED) {
        nu_transform_get_left(&_module.transform, temp);
        nu_vec3f_add(direction, temp, direction);
    }
    if (right_state & NU_BUTTON_PRESSED) {
        nu_transform_get_right(&_module.transform, temp);
        nu_vec3f_add(direction, temp, direction);
    }
    if (_module.free_mode) {
        if (up_state & NU_BUTTON_PRESSED) {
            nu_transform_get_up(&_module.transform, temp);
            nu_vec3f_add(direction, temp, direction);
        }
        if (down_state & NU_BUTTON_PRESSED) {
            nu_transform_get_down(&_module.transform, temp);
            nu_vec3f_add(direction, temp, direction);
        }
    } else {
        if (up_state & NU_BUTTON_PRESSED) {
            nu_vec3f_add(direction, NU_VEC3F_UP, direction);
        }
        if (down_state & NU_BUTTON_PRESSED) {
            nu_vec3f_add(direction, NU_VEC3F_DOWN, direction);
        }
    }
    nu_vec3f_normalize(direction);

    /* speed */
    float speed = NORMAL_SPEED;
    if (slow_mode_state & NU_BUTTON_PRESSED) speed = SLOW_SPEED;
    if (fast_mode_state & NU_BUTTON_PRESSED) speed = FAST_SPEED;

    /* apply translation */
    nu_vec3f_muls(direction, delta * speed, direction);
    nu_vec3f_add(_module.transform.translation, direction, _module.transform.translation);

    /* fov */
    nu_vec2f_t scroll;
    nu_input_get_mouse_scroll(scroll);
    if (scroll[1] != 0.0f) {
        _module.fov += scroll[1] > 0.0f ? -ZOOM_SPEED : ZOOM_SPEED;
        _module.fov = NU_MAX(0.1f, NU_MIN(179.9f, _module.fov));
    }

    /* get mouse motion */
    nu_vec2f_t motion;
    nu_input_get_mouse_motion(motion);

    /* apply rotation */
    if (_module.free_mode) {
        if (motion[0] != 0.0f) {
            nu_quatf_mul_axis(_module.transform.rotation, -nu_radian(motion[0]) * ROTATION_SENSIBILITY,
                NU_VEC3F_UP, _module.transform.rotation);
        }
        if (motion[1] != 0.0f) {
            nu_quatf_mul_axis(_module.transform.rotation, nu_radian(motion[1]) * ROTATION_SENSIBILITY,
                NU_VEC3F_LEFT, _module.transform.rotation);
        }
        if (roll_left_state & NU_BUTTON_PRESSED) {
            nu_quatf_mul_axis(_module.transform.rotation, -nu_radian(ROLL_SPEED) * delta,
                NU_VEC3F_FORWARD, _module.transform.rotation);
        }
        if (roll_right_state & NU_BUTTON_PRESSED) {
            nu_quatf_mul_axis(_module.transform.rotation, nu_radian(ROLL_SPEED) * delta,
                NU_VEC3F_FORWARD, _module.transform.rotation);
        }
    } else {
        if (motion[0] != 0.0f) {
            _module.yaw += motion[0] * ROTATION_SENSIBILITY;
        }
        if (motion[1] != 0.0f) {
            _module.pitch += motion[1] * ROTATION_SENSIBILITY;
        }

        if (_module.pitch < -90.0f) _module.pitch = -90.0f;
        if (_module.pitch > 90.0f) _module.pitch = 90.0f;

        nu_quatf_from_axis(-nu_radian(_module.yaw), NU_VEC3F_UP, _module.transform.rotation);
        nu_quatf_mul_axis(_module.transform.rotation, nu_radian(_module.pitch),
                NU_VEC3F_LEFT, _module.transform.rotation);
    }

    /* update camera */
    nu_vec3f_t forward, up;
    nu_transform_get_forward(&_module.transform, forward);
    nu_transform_get_up(&_module.transform, up);

    nu_renderer_camera_set_view(_module.camera, _module.transform.translation, forward, up);
    nu_renderer_camera_set_fov(_module.camera, nu_radian(_module.fov));

    return NU_SUCCESS;
}