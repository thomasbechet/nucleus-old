#pragma once

#include "../utility/internalptr.hpp"

#include <vulkan/vulkan.hpp>

#include <memory>

namespace nuvk
{
    class Context
    {
    public:
        inline constexpr auto Section = "CONTEXT";
        vk::UniqueDevice &getDevice();
        vk::SurfaceKHR &getSurface();

    private:
        struct Internal;
        InternalPtr<Internal> internal;

    // public:
    //     static std::vector<const char*> GetRequiredExtensions(GLFWInterface &glfwInterface, bool enableValidationLayers);
    //     static std::vector<const char*> GetRequiredValidationLayers();
    //     static std::vector<const char*> GetRequiredDeviceExtensions();

    // public:
    //     Context(bool enableValidationLayers = true);
    //     ~Context();
        
    // public:
    //     const vk::Device &getDevice() const;
    //     const vk::Queue &getGraphicsQueue() const;
    //     const vk::Queue &getPresentQueue() const;

    // private:
    //     void createInstance();
    //     void setupDebugCallback();
    //     void createSurface();
    //     void pickPhysicalDevice();
    //     void createLogicalDevice();

    // private:
    //     std::unique_ptr<GLFWInterface> m_glfwInterface;
    //     vk::UniqueInstance m_instance;
    //     std::unique_ptr<DebugUtilsMessenger> m_debugUtilsMessenger;
    //     VkSurfaceKHR m_surface;

    //     vk::PhysicalDevice m_physicalDevice;
    //     vk::UniqueDevice m_device;

    //     vk::Queue m_graphicsQueue;
    //     vk::Queue m_presentQueue;

    //     bool m_useValidationLayers;
    };
}