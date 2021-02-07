#include "windowinterface.hpp"

#include <stdexcept>

using namespace nu::softrast;

WindowInterface::WindowInterface()
{
    if (NU_MODULE_LOAD_INTERFACE(NUGLFW_MODULE_NAME, NUGLFW_WINDOW_INTERFACE_NAME, &m_glfwInterface) != NU_SUCCESS) {
        throw new std::runtime_error("Softrast failed to load glfw interface.");
    }
}

void WindowInterface::presentSurface(const Vector2u &size, uint32_t *data) const
{
    m_glfwInterface.present_surface(size.data, data);
}
void WindowInterface::swapBuffers() const
{
    m_glfwInterface.swap_buffers();
}