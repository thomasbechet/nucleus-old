#include "surface.hpp"

#include "../utility/glfwinterface.hpp"

using namespace nuvk;

namespace
{
    static VkSurfaceKHR CreateSurface(
        vk::UniqueInstance &instance,
        GLFWInterface &glfwInterface
    )
    {
        VkSurfaceKHR rawSurface;
        VkInstance vkinstance = *instance;
        if (glfwInterface->create_window_surface(&vkinstance, &rawSurface) != NU_SUCCESS) {
            Engine::Interrupt("Failed to create surface from GLFW.");
        }
        return rawSurface;
    }
}

struct Surface::Internal
{
    vk::SurfaceKHR surface;

    Internal(
        vk::UniqueInstance &instance,
        GLFWInterface &glfwInterface
    )
    {
        surface = ::CreateSurface(instance, glfwInterface);
    }
    ~Internal()
    {

    }
};

Surface::Surface(
    vk::UniqueInstance &instance,
    GLFWInterface &glfwInterface
) : internal(MakeInternalPtr<Internal>(instance, glfwInterface)) {}

vk::SurfaceKHR &Surface::getSurface()
{
    return internal->surface;
}