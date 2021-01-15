#include "shaderhelper.hpp"

#include "logger.hpp"

#include <fstream>
#include <vector>

using namespace nuvk;

VkShaderModule ShaderHelper::CreateShaderModule(VkDevice device, const std::string &filename)
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

    VkShaderModuleCreateInfo info{};
    info.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    info.codeSize = buffer.size();
    info.pCode    = reinterpret_cast<const uint32_t*>(buffer.data());

    VkShaderModule module;
    vkCreateShaderModule(device, &info, nullptr, &module);
    return module;
}