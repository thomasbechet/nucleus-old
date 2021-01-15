#pragma once

#include "../../glfw/module/interface.h"

namespace nusr
{
    class WindowInterface
    {
    public:
        WindowInterface();

        void presentSurface(uint32_t width, uint32_t height, uint32_t *data) const;
        void swapBuffers() const;

    private:
        nuglfw_window_interface_t m_glfwInterface;
    };
}