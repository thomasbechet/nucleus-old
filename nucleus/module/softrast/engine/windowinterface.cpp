#include <nucleus/module/softrast/engine/windowinterface.hpp>

#include <stdexcept>

using namespace nu::softrast;

WindowInterface::WindowInterface()
{
    nu_module_handle_t module;
    if (nu_module_get_by_name(NUGLFW_MODULE_NAME, &module) != NU_SUCCESS) throw new std::runtime_error("Softrast failed to load glfw interface.");
    if (nu_module_load_interface(module, NUGLFW_WINDOW_INTERFACE_NAME, &m_glfwInterface) != NU_SUCCESS) throw new std::runtime_error("Softrast failed to load glfw interface.");
}

void WindowInterface::presentSurface(const Vector2u &size, uint32_t *data) const
{
    m_glfwInterface.present_surface(size.data, data);
}
void WindowInterface::swapBuffers() const
{
    m_glfwInterface.swap_buffers();
}