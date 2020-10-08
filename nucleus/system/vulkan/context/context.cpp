#include "context.hpp"

using namespace nuvk;

Context::Context(
    const std::vector<const char*> &extensions,
    const std::vector<const char*> &deviceExtensions,
    const std::vector<const char*> &validationLayers
) : Loggable("Context")
{
    Logger::Warning(this, "hello");

    // Create GLFW interface
    m_glfwInterface = std::make_unique<GLFWInterface>();

    // Recover required glfw interface
    uint32_t glfwExtensionCount = 0;
    const char **glfwExtensions;
    glfwExtensions = (*m_glfwInterface)->get_required_instance_extensions(&glfwExtensionCount);
    std::vector<const char*> requiredExtensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    // Save required extensions and layers
    m_requiredExtensions = requiredExtensions;
    m_requiredDeviceExtensions = deviceExtensions;
    m_requiredValidationLayers = validationLayers;

    createInstance();
    setupDebugCallback();
    createSurface();
    pickPhysicalDevice();
    createLogicalDevice();
}
Context::~Context()
{

}

vk::Device &Context::getDevice() const
{
    return *m_device;
}
vk::Queue &Context::getGraphicsQueue() const
{
    return *m_graphicsQueue;
}
vk::Queue &Context::getPresentQueue() const
{
    return *m_presentQueue;
}

void Context::createInstance()
{

}
void Context::setupDebugCallback()
{

}
void Context::createSurface()
{

}
void Context::pickPhysicalDevice()
{

}
void Context::createLogicalDevice()
{

}