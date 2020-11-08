#include "engine.hpp"

#include <nucleus/nucleus.h>
#include "../utility/logger.hpp"

#include "../context/debugutilsmessenger.hpp"
#include "../context/instance.hpp"
#include "../context/physicaldevice.hpp"
#include "../context/device.hpp"
#include "../context/surface.hpp"
#include "../context/memoryallocator.hpp"
#include "../context/rendercontext.hpp"
#include "../context/pipeline.hpp"
#include "../context/windowinterface.hpp"
#include "../context/buffer.hpp"

using namespace nuvk;

namespace
{
    
}

struct Engine::Internal
{
    std::unique_ptr<WindowInterface> windowInterface;
    std::unique_ptr<Instance> instance;
    std::unique_ptr<DebugUtilsMessenger> debugUtilsMessenger;
    std::unique_ptr<Surface> surface;
    std::unique_ptr<PhysicalDevice> physicalDevice;
    std::unique_ptr<Device> device;
    std::unique_ptr<CommandPool> graphicsCommandPool;
    std::unique_ptr<MemoryAllocator> memoryAllocator;
    std::unique_ptr<RenderContext> renderContext;
    std::unique_ptr<Pipeline> pipeline;
    std::unique_ptr<Buffer> vertexBuffer;
    std::unique_ptr<Buffer> indiceBuffer;

    bool renderContextOutOfDate = false;

    const bool enableValidationLayers = true; 

    Internal()
    {
        createContext();
        createAssets();
        createRenderContext();
    }
    ~Internal()
    {
        device->getDevice().waitIdle();
        destroyRenderContext();
        destroyAssets();
        destroyContext();
    }

    void createContext()
    {
        // get window interface
        Logger::Info(Engine::Section, "Getting window interface...");
        windowInterface = std::make_unique<WindowInterface>();
        
        // create instance
        Logger::Info(Engine::Section, "Creating instance...");
        instance = std::make_unique<Instance>(*windowInterface, enableValidationLayers);
        
        if (enableValidationLayers) {
            // create debug utils messenger
            Logger::Info(Engine::Section, "Creating debug utils messenger...");
            debugUtilsMessenger = std::make_unique<DebugUtilsMessenger>(*instance);
        }
        
        // create surface
        Logger::Info(Engine::Section, "Creating surface...");
        surface = std::make_unique<Surface>(
            *instance,
            *windowInterface
        );

        // create physical device
        Logger::Info(Engine::Section, "Picking physical device...");
        physicalDevice = std::make_unique<PhysicalDevice>(
            *instance,
            *surface
        );

        // create device
        Logger::Info(Engine::Section, "Creating device...");
        device = std::make_unique<Device>(
            *physicalDevice,
            *surface,
            enableValidationLayers
        );

        // create commandpool
        Logger::Info(Engine::Section, "Create command pool...");
        graphicsCommandPool = std::make_unique<CommandPool>(
            *device,
            device->getGraphicsQueueIndex()
        );

        // create memory allocator
        Logger::Info(Engine::Section, "Create memory allocator...");
        memoryAllocator = std::make_unique<MemoryAllocator>(
            *instance,
            *physicalDevice,
            *device
        );
    }
    void createAssets()
    {
        const std::vector<Vertex> vertices = {
            {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
            {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
            {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
            {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
        };

        // vertexBuffer = std::make_unique<Buffer>(
        //     *physicalDevice,
        //     *device,
        //     vk::BufferUsageFlagBits::eVertexBuffer,
        //     vk::MemoryPropertyFlagBits::eHostVisible |
        //     vk::MemoryPropertyFlagBits::eHostCoherent,
        //     sizeof(Vertex) * vertices.size(),
        //     vertices.data()
        // );

        vertexBuffer = std::make_unique<Buffer>(
            Buffer::CreateDeviceLocalBuffer(
                *physicalDevice,
                *device,
                *graphicsCommandPool,
                vk::BufferUsageFlagBits::eVertexBuffer,
                sizeof(Vertex) * vertices.size(),
                vertices.data()
            )
        );

        const std::vector<uint32_t> indices = {
            0, 1, 2, 2, 3, 0
        };

        indiceBuffer = std::make_unique<Buffer>(
            Buffer::CreateDeviceLocalBuffer(
                *physicalDevice,
                *device,
                *graphicsCommandPool,
                vk::BufferUsageFlagBits::eIndexBuffer,
                sizeof(uint32_t) * indices.size(),
                indices.data()
            )
        );
    }
    void createRenderContext()
    {
        // create rendercontext
        Logger::Info(Engine::Section, "Creating render context...");
        uint32_t width, height;
        nu_window_get_size(&width, &height);
        renderContext = std::make_unique<RenderContext>(
            *device,
            *physicalDevice,
            *surface,
            *graphicsCommandPool,
            width, height
        );

        // create pipeline
        Logger::Info(Engine::Section, "Creating pipeline...");
        pipeline = std::make_unique<Pipeline>(
            device->getDevice(),
            renderContext->getRenderPass(),
            renderContext->getSwapchainExtent()
        );
    }
    void destroyRenderContext()
    {
        pipeline.reset();
        renderContext.reset();
    }
    void destroyAssets()
    {
        indiceBuffer.reset();
        vertexBuffer.reset();
    }
    void destroyContext()
    {
        memoryAllocator.reset();
        graphicsCommandPool.reset();
        device.reset();
        physicalDevice.reset();
        surface.reset();
        if (enableValidationLayers) {
            debugUtilsMessenger.reset();
        }
        instance.reset();
        windowInterface.reset();
    }

    bool tryRender()
    {
        if (!renderContext->beginRender()) {
            return false;
        }

        const vk::CommandBuffer &commandBuffer {renderContext->getActiveCommandBuffer()};
        commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline->getPipeline());
        const vk::Buffer vertexBuffers[] = {vertexBuffer->getBuffer()};
        const vk::DeviceSize offsets[] = {0};
        commandBuffer.bindVertexBuffers(0, 1, vertexBuffers, offsets);
        commandBuffer.bindIndexBuffer(indiceBuffer->getBuffer(), 0, vk::IndexType::eUint32);
        commandBuffer.drawIndexed(6, 1, 0, 0, 0);
        //commandBuffer.draw(3, 1, 0, 0);

        if (!renderContext->endRender()) {
            return false;
        }

        return true;
    }
    void render()
    {
        if (!renderContextOutOfDate) {
            renderContextOutOfDate = !tryRender();
        } else {
            device->getDevice().waitIdle();
            
            uint32_t width, height;
            nu_window_get_size(&width, &height);
            if (width != 0 && height != 0) {
                destroyRenderContext();
                createRenderContext();
                renderContextOutOfDate = false;
            }
        }
    }
};

Engine::Engine() : internal(MakeInternalPtr<Internal>()) {}

void Engine::render()
{
    internal->render();
}

void Engine::Interrupt(std::string_view msg)
{
    nu_interrupt(("[NUVK] " + std::string(msg) + "\n").c_str());
}