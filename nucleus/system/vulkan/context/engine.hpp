#pragma once

#include "../utility/glfwinterface.hpp"
#include "../utility/debugutilsmessenger.hpp"

#include <vulkan/vulkan.hpp>

#include <optional>

namespace nuvk
{
    struct QueueFamilyIndices
    {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        bool isComplete() {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

    struct SwapChainSupportDetails
    {
        vk::SurfaceCapabilitiesKHR capabilities;
        std::vector<vk::SurfaceFormatKHR> formats;
        std::vector<vk::PresentModeKHR> presentModes;
    };

    class Engine
    {
    public:
        Engine();
        ~Engine();

        void render();

    public:
        static void Interrupt(std::string msg);
        static std::vector<const char*> GetRequiredExtensions(GLFWInterface &glfwInterface);
        static std::vector<const char*> GetRequiredValidationLayers();
        static std::vector<const char*> GetRequiredDeviceExtensions();
        static bool CheckValidationLayerSupport();
        static bool CheckDeviceExtensionSupport(vk::PhysicalDevice device);
        static bool IsDeviceSuitable(vk::PhysicalDevice device, VkSurfaceKHR surface);
        static QueueFamilyIndices FindQueueFamilies(vk::PhysicalDevice device, VkSurfaceKHR surface);
        static SwapChainSupportDetails QuerySwapChainSupport(vk::PhysicalDevice device, VkSurfaceKHR surface); 
        static vk::SurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR> &availableFormats);
        static vk::PresentModeKHR ChooseSwapPresentMode(const std::vector<vk::PresentModeKHR> &availablePresentModes);
        static vk::Extent2D ChooseSwapExtent(const vk::SurfaceCapabilitiesKHR &capabilities);

        static constexpr bool USE_VALIDATION_LAYERS = true;
        static constexpr uint32_t WIDTH = 900;
        static constexpr uint32_t HEIGHT = 450;

    private:
        void initialize();
        void terminate();

        void createInstance();
        void setupDebugCallback();
        void createSurface();
        void pickPhysicalDevice();
        void createLogicalDevice();
        void createSwapChain();
        void createImageViews();

        GLFWInterface m_glfwInterface;
        vk::UniqueInstance m_instance;
        VkSurfaceKHR m_surface;
        std::unique_ptr<DebugUtilsMessenger> m_debugUtilsMessenger;
        
        vk::PhysicalDevice m_physicalDevice;
        vk::UniqueDevice m_device;
        
        vk::Queue m_graphicsQueue;
        vk::Queue m_presentQueue;

        vk::SwapchainKHR m_swapChain;
        std::vector<vk::Image> m_swapChainImages;
        vk::Format m_swapChainImageFormat;
        vk::Extent2D m_swapChainExtent;
        std::vector<vk::UniqueImageView> m_swapChainImageViews;
    };
}