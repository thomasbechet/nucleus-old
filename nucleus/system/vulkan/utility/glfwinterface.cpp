#include "glfwinterface.hpp"

#include "../engine/engine.hpp"

using namespace nuvk;

GLFWInterface::GLFWInterface()
{
    nu_module_handle_t window_module = nu_system_window_get_module_handle();
    if (nu_module_get_id(window_module) != NUGLFW_MODULE_ID) {
        Engine::Interrupt("Vulkan requires GLFW module to work.");
    }

    nuglfw_window_interface_loader_pfn_t load_interface;
    if (nu_module_load_function(window_module, NUGLFW_WINDOW_INTERFACE_LOADER_NAME, (nu_pfn_t*)&load_interface) != NU_SUCCESS) {
        Engine::Interrupt("Vulkan failed to load glfw loader.");
    }

    if (load_interface(&m_interface) != NU_SUCCESS) {
        Engine::Interrupt("Vulkan rasterizer failed to load glfw interface.");
    }
}

nuglfw_window_interface_t *GLFWInterface::operator->()
{
    return &m_interface;
}