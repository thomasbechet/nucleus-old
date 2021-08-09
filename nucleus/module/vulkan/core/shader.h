#ifndef NUVK_SHADER_MANAGER_H
#define NUVK_SHADER_MANAGER_H

#include <nucleus/module/vulkan/core/context.h>
#include <shaderc/shaderc.h>

#define NUVK_SHADER_INCLUDE_DEBUG_SYMBOLS true

typedef struct {
    shaderc_compiler_t compiler;
    shaderc_compile_options_t options;
} nuvk_shader_manager_t;

nu_result_t nuvk_shader_manager_initialize(nuvk_shader_manager_t *manager);
nu_result_t nuvk_shader_manager_terminate(nuvk_shader_manager_t *manager);

nu_result_t nuvk_shader_module_create_from_glsl(
    const nuvk_context_t *context,
    const nuvk_shader_manager_t *manager,
    const char *filename,
    VkShaderStageFlags stage,
    VkShaderModule *module
);

#endif