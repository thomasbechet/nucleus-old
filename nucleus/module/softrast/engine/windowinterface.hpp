#pragma once

#include <nucleus/nucleus.hpp>

#include <nucleus/module/glfw/module/interface.h>

namespace nu::softrast
{
    class WindowInterface
    {
    public:
        WindowInterface();

        void presentSurface(const Vector2u &size, uint32_t *data) const;
        void swapBuffers() const;

    private:
        nuglfw_window_interface_t m_glfwInterface;
    };
}