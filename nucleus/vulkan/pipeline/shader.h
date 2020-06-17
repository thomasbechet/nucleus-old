#ifndef NUVK_SHADER_H
#define NUVK_SHADER_H

#include "../renderer.h"

nu_result_t nuvk_shader_module_create(VkShaderModule *shader_module, const char *file);
nu_result_t nuvk_shader_module_destroy(VkShaderModule shader_module);

#endif