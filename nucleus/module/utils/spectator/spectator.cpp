#include <nucleus/module/utils/spectator/spectator.hpp>

#include <memory>

using namespace nu::utility;

typedef struct {
    std::unique_ptr<SpectatorPlugin> plugin;
} nu_plugin_data_t;

static nu_plugin_data_t _data;

nu_result_t nuutils_spectator_plugin_initialize(void)
{
    _data.plugin = std::make_unique<SpectatorPlugin>();
    return NU_SUCCESS;
}
nu_result_t nuutils_spectator_plugin_terminate(void)
{
    _data.plugin.reset();
    return NU_SUCCESS;
}
nu_result_t nuutils_spectator_plugin_update(void)
{
    _data.plugin->update();
    return NU_SUCCESS;
}

SpectatorPlugin::SpectatorPlugin()
{
    // Create camera
    // nu_renderer_camera_create_info_t info = {};
    // if (nu_renderer_camera_create(&m_camera, &info) != NU_SUCCESS) {
    //     throw std::runtime_error("Failed to load camera.");
    // }

    m_transform.setTranslation({0.0f, 0.0f, 5.0f});
}
SpectatorPlugin::~SpectatorPlugin()
{
    // Destroy camera
    // nu_renderer_camera_destroy(m_camera);
}
void SpectatorPlugin::update()
{
    // Check cursor mode
    nu_cursor_mode_t cursor_mode;
    nu_input_get_cursor_mode(&cursor_mode);
    if (!(cursor_mode & NU_CURSOR_MODE_DISABLE)) return;

    // Get deltatime
    float delta = nu_context_get_delta_time();
    
    // Get mouse motion
    Vector2f motion;
    nu_input_get_mouse_motion(motion.data);

    // Get inputs
    nu_button_state_t slowModeState, fastModeState, changeModeState;
    nu_button_state_t forwardState, backwardState, leftState, rightState, upState, downState;
    nu_button_state_t rollLeftState, rollRightState;
    nu_input_get_keyboard_state(NU_KEYBOARD_LCONTROL, &slowModeState);
    nu_input_get_keyboard_state(NU_KEYBOARD_LSHIT, &fastModeState);
    nu_input_get_keyboard_state(NU_KEYBOARD_C, &changeModeState);
    nu_input_get_keyboard_state(NU_KEYBOARD_W, &forwardState);
    nu_input_get_keyboard_state(NU_KEYBOARD_S, &backwardState);
    nu_input_get_keyboard_state(NU_KEYBOARD_A, &leftState);
    nu_input_get_keyboard_state(NU_KEYBOARD_D, &rightState);
    nu_input_get_keyboard_state(NU_KEYBOARD_X, &upState);
    nu_input_get_keyboard_state(NU_KEYBOARD_Z, &downState);
    nu_input_get_keyboard_state(NU_KEYBOARD_Q, &rollLeftState);
    nu_input_get_keyboard_state(NU_KEYBOARD_E, &rollRightState);

    // Update mode
    if (changeModeState & NU_BUTTON_JUST_PRESSED) m_freeMode = !m_freeMode;

    // Translation
    Vector3f direction;
    if (forwardState & NU_BUTTON_PRESSED)  direction += m_transform.getForwardVector();
    if (backwardState & NU_BUTTON_PRESSED) direction += m_transform.getBackwardVector();
    if (leftState & NU_BUTTON_PRESSED)     direction += m_transform.getLeftVector();
    if (rightState & NU_BUTTON_PRESSED)    direction += m_transform.getRightVector();
    if (m_freeMode) {
        if (upState & NU_BUTTON_PRESSED)   direction += m_transform.getUpVector();
        if (downState & NU_BUTTON_PRESSED) direction += m_transform.getDownVector();
    } else {
        if (upState & NU_BUTTON_PRESSED)   direction += Vector3f::up();
        if (downState & NU_BUTTON_PRESSED) direction += Vector3f::down();
    }
    direction.normalize();

    // Speed
    float speed = m_normalSpeed;
    if (slowModeState & NU_BUTTON_PRESSED) speed = m_slowSpeed;
    if (fastModeState & NU_BUTTON_PRESSED) speed = m_fastSpeed;

    // Apply translation
    //Vector3f position = m_transform.getTranslation();
    //Vector3f target = position + direction;
    //m_transform.setTranslation(Vector3f::lerp(position, target, delta * speed));
    m_transform.translate(direction * delta * speed);

    // Fov
    nu_vec2f_t scroll;
    nu_input_get_mouse_scroll(scroll);
    if (scroll[1] != 0.0f) {
        m_fov += scroll[1] > 0.0f ? -m_zoomSpeed : m_zoomSpeed;
        m_fov = NU_MAX(0.1f, NU_MIN(179.9f, m_fov));
    }

    // Apply Rotation
    if (m_freeMode) {
        if (motion.x != 0.0f)
            m_transform.rotate(-nu_radian(motion.x) * m_rotationSensibility, Vector3f::up());
        if (motion.y != 0.0f)
            m_transform.rotate(nu_radian(motion.y) * m_rotationSensibility, Vector3f::left());
        if (rollLeftState & NU_BUTTON_PRESSED)
            m_transform.rotate(-nu_radian(m_rollSpeed) * delta, Vector3f::forward());
        if (rollRightState & NU_BUTTON_PRESSED)
            m_transform.rotate(nu_radian(m_rollSpeed) * delta, Vector3f::forward());
    } else {
        if (motion.x != 0.0f)
            m_yaw += motion.x * m_rotationSensibility;
        if (motion.y != 0.0f)
            m_pitch += motion.y * m_rotationSensibility;

        if (m_pitch < -90.0f) m_pitch = -90.0f;
        if (m_pitch > 90.0f) m_pitch = 90.0f;

        m_transform.setRotation(-nu_radian(m_yaw), Vector3f::up());
        m_transform.rotate(nu_radian(m_pitch), Vector3f::left());
    }

    // Update camera
    nu_renderer_camera_set_view(m_camera, 
        m_transform.getTranslation(), 
        m_transform.getForwardVector(),
        m_transform.getUpVector());
    nu_renderer_camera_set_fov(m_camera, nu_radian(m_fov));
}