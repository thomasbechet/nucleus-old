#pragma once

#include <nucleus/utility/nucleus.hpp>

/* plugin interface */
nu_result_t nuutils_spectator_plugin_initialize(void);
nu_result_t nuutils_spectator_plugin_terminate(void);
nu_result_t nuutils_spectator_plugin_update(void);

namespace nu::utility
{
    class SpectatorPlugin
    {
    public:
        SpectatorPlugin();
        ~SpectatorPlugin();
        void update();

    private:
        nu_renderer_camera_handle_t m_camera;
        Transform m_transform;
        Vector3f m_velocity;

        float m_normalSpeed         = 20.0f  / 1000.0f;
        float m_slowSpeed           = 2.0f   / 1000.0f;
        float m_fastSpeed           = 100.0f / 1000.0f;
        float m_rollSpeed           = 90.0f  / 1000.0f;
        float m_rotationSensibility = 0.1f;
        float m_zoomSpeed           = 5.0f;
        bool m_freeMode             = false;
        
        float m_yaw   = 0.0f;
        float m_pitch = 0.0f;
        float m_fov   = 80.0f;
    };
}