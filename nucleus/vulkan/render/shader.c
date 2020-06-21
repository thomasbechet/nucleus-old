#include <stdio.h>

#include "../common/logger.h"
#include "../context/context.h"

static nu_result_t read_shader_file(const char *path, char **buffer, uint32_t *size)
{
    FILE *file;
    
    file = fopen(path, "rb");
    fseek(file, 0, SEEK_END);
    *size = ftell(file);
    fseek(file, 0, SEEK_SET);

    *buffer = (char*)nu_malloc(sizeof(char) * (*size));

    if (*buffer == NULL) {
        nu_warning(NUVK_VULKAN_LOG_NAME"Failed to load file %s.\n", path);
        return NU_FAILURE;
    }

    if (fread(*buffer, sizeof(char), *size, file) != (*size)) {
        nu_warning(NUVK_VULKAN_LOG_NAME"Failed to read file %s.\n", path);
        nu_free(*buffer);
        return NU_FAILURE;
    }

    fclose(file);

    return NU_SUCCESS;
}

nu_result_t nuvk_shader_module_create(VkShaderModule *shader_module, const char *file)
{
    const nuvk_context_t *ctx = nuvk_context_get();

    char *code;
    uint32_t size;

    if (read_shader_file(file, &code, &size) != NU_SUCCESS) {
        return NU_FAILURE;
    }

    VkShaderModuleCreateInfo create_info;
    memset(&create_info, 0, sizeof(VkShaderModuleCreateInfo));
    create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    create_info.codeSize = size;
    create_info.pCode = (uint32_t*)code;

    if (vkCreateShaderModule(ctx->device, &create_info, NULL, shader_module) != VK_SUCCESS) {
        nu_warning(NUVK_VULKAN_LOG_NAME"Failed to create shader module.\n");
        nu_free(code);
        return NU_FAILURE;
    }

    nu_free(code);

    return NU_SUCCESS;
}
nu_result_t nuvk_shader_module_destroy(VkShaderModule shader_module)
{
    const nuvk_context_t *ctx = nuvk_context_get();

    vkDestroyShaderModule(ctx->device, shader_module, NULL);

    return NU_SUCCESS;
}