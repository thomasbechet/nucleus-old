#include "surface.hpp"

using namespace nuvk;

namespace
{
    static VkSurfaceKHR CreateInstance()
    {
        VkSurfaceKHR rawSurface;
        VkInstance instance = *m_instance;
        if ((*m_glfwInterface)->create_window_surface(&instance, &rawSurface) != NU_SUCCESS) {
            Engine::Interrupt("Failed to create surface from GLFW.");
        }
        m_surface = rawSurface;
    }
}

struct Surface::Internal
{
    Internal()
    {

    }
    ~Internal()
    {

    }
};

Surface::Surface() : internal(MakeInternalPtr<Internal>()) {}

vk::SurfaceKHR &Surface::getSurface()
{

}