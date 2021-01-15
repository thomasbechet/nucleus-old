#include "windowinterface.hpp"

#include "../engine/engine.hpp"

#include "../../glfw/module/interface.h"

using namespace nuvk;

namespace
{

}

struct WindowInterface::Internal
{
    nuglfw_window_interface_t m_interface;

    Internal()
    {
        nu_module_handle_t window_module = nu_system_window_get_module_handle();
        if (nu_module_get_id(window_module) != NUGLFW_MODULE_ID) {
            Engine::Interrupt(WindowInterface::Section, "Vulkan requires GLFW module to work.");
        }

        nuglfw_window_interface_loader_pfn_t load_interface;
        if (nu_module_load_function(window_module, NUGLFW_WINDOW_INTERFACE_LOADER_NAME, (nu_pfn_t*)&load_interface) != NU_SUCCESS) {
            Engine::Interrupt(WindowInterface::Section, "Vulkan failed to load glfw loader.");
        }

        if (load_interface(&m_interface) != NU_SUCCESS) {
            Engine::Interrupt(WindowInterface::Section, "Vulkan rasterizer failed to load glfw interface.");
        }
    }
    ~Internal()
    {

    }

    std::vector<const char*> getRequiredInstanceExtensions() const
    {
        uint32_t glfwExtensionCount = 0;
        const char **glfwExtensions;
        glfwExtensions = m_interface.get_required_instance_extensions(&glfwExtensionCount);

        std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
        
        return extensions;
    }
    VkSurfaceKHR createWindowSurface(VkInstance instance) const
    {
        VkSurfaceKHR rawSurface;
        if (m_interface.create_window_surface(&instance, &rawSurface) != NU_SUCCESS) {
            Engine::Interrupt(WindowInterface::Section, "Failed to create surface from GLFW.");
        }
        return rawSurface;
    }
};

WindowInterface::WindowInterface() : internal(MakeInternalPtr<Internal>()) {}

std::vector<const char*> WindowInterface::getRequiredInstanceExtensions() const
{
    return internal->getRequiredInstanceExtensions();
}
VkSurfaceKHR WindowInterface::createWindowSurface(VkInstance instance) const
{
    return internal->createWindowSurface(instance);
}