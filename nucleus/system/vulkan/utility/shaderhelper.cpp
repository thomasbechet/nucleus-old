#include "shaderhelper.hpp"

#include "logger.hpp"

#include <fstream>

using namespace nuvk;

vk::UniqueShaderModule ShaderHelper::CreateShaderModule(const vk::Device &device, const std::string &filename)
{
    std::ifstream file(filename, std::ios::ate | std::ios::binary);
    if (!file.is_open()) {
        Logger::Warning("Failed to open file '" + filename + "'");
        throw std::runtime_error("Failed to load.");
    }

    uint32_t fileSize = file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();

    return device.createShaderModuleUnique({
        vk::ShaderModuleCreateFlagBits(),
        buffer.size(),
        reinterpret_cast<const uint32_t*>(buffer.data())
    });
}