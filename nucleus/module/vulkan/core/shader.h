#ifndef NUVK_SHADER_MANAGER_H
#define NUVK_SHADER_MANAGER_H

#include <nucleus/module/vulkan/core/context.h>
#include <shaderc/shaderc.h>

#define NUVK_SHADER_INCLUDE_DEBUG_SYMBOLS true
#define NUVK_SHADER_DUMP_SOURCES          true

typedef struct {
    shaderc_compiler_t compiler;
    shaderc_compile_options_t options;
    nu_array_t injectors;
    nu_string_t placeholder_source;
} nuvk_shader_manager_t;

nu_result_t nuvk_shader_manager_initialize(nuvk_shader_manager_t *manager);
nu_result_t nuvk_shader_manager_terminate(nuvk_shader_manager_t *manager);

nu_result_t nuvk_shader_module_create_from_glsl_source(
    const nuvk_context_t *context,
    const nuvk_shader_manager_t *manager,
    VkShaderStageFlags stage,
    nu_string_t glsl_source,
    const char *identifier,
    VkShaderModule *module
);

#endif