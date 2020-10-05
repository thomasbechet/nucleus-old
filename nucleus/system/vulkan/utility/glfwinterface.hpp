#pragma once

#include "../../glfw/module/interface.h"

namespace nuvk
{
    class GLFWInterface
    {
    public:
        GLFWInterface();

        nuglfw_window_interface_t *operator->();
    
    private:
        nuglfw_window_interface_t m_interface;
    };
}