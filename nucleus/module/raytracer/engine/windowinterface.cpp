#include "windowinterface.hpp"

#include <stdexcept>

using namespace nurt;

WindowInterface::WindowInterface()
{
    nu_module_handle_t window_module = nu_system_window_get_module_handle();
    if (nu_module_get_id(window_module) != NUGLFW_MODULE_ID) {
        throw new std::runtime_error("Raytracer requires GLFW module to work.");
    }

    nuglfw_window_interface_loader_pfn_t load_interface;
    if (nu_module_load_function(window_module, NUGLFW_WINDOW_INTERFACE_LOADER_NAME, (nu_pfn_t*)&load_interface) != NU_SUCCESS) {
        throw new std::runtime_error("Raytracer failed to load glfw loader.");
    }

    if (load_interface(&m_glfwInterface) != NU_SUCCESS) {
        throw new std::runtime_error("Raytracer failed to load glfw interface.");
    }
}

void WindowInterface::presentSurface(uint32_t width, uint32_t height, uint32_t *data) const
{
    m_glfwInterface.present_surface(width, height, data);
}
void WindowInterface::swapBuffers() const
{
    m_glfwInterface.swap_buffers();
}