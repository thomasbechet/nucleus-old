#pragma once

#include "../utility/glfwinterface.hpp"
#include "../utility/debugutilsmessenger.hpp"
#include "../utility/loggable.hpp"

#include <vulkan/vulkan.hpp>

#include <memory>

namespace nuvk
{
    class Context : public Loggable
    {
    public:
        Context(
            const std::vector<const char*> &extensions,
            const std::vector<const char*> &deviceExtensions,
            const std::vector<const char*> &validationLayers
        );
        ~Context();
        
    public:
        vk::Device &getDevice() const;
        vk::Queue &getGraphicsQueue() const;
        vk::Queue &getPresentQueue() const;

    private:
        void createInstance();
        void setupDebugCallback();
        void createSurface();
        void pickPhysicalDevice();
        void createLogicalDevice();

    private:

    private:
        std::vector<const char*> m_requiredExtensions;
        std::vector<const char*> m_requiredDeviceExtensions;
        std::vector<const char*> m_requiredValidationLayers;

        std::unique_ptr<GLFWInterface> m_glfwInterface;
        vk::UniqueInstance m_instance;
        std::unique_ptr<DebugUtilsMessenger> m_debugUtilsMessenger;
        VkSurfaceKHR m_surface;

        vk::PhysicalDevice m_physicalDevice;
        vk::UniqueDevice m_device;

        vk::Queue m_graphicsQueue;
        vk::Queue m_presentQueue;
    };
}