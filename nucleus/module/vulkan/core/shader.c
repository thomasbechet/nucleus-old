#include <nucleus/module/vulkan/core/shader.h>

#include <shaderc/shaderc.h>

static shaderc_include_result *resolver(
    void *user,
    const char *requested_source,
    int type,
    const char *requesting_source,
    size_t include_depth
)
{
    shaderc_include_result *include_result = (shaderc_include_result*)nu_malloc(sizeof(shaderc_include_result));

    nu_path_t path;
    nu_path_allocate_cstr(requested_source, &path);

    uint32_t nbytes;
    char *buf;
    nu_result_t result = nu_file_read_allbytes(nu_path_get_cstr(path), &nbytes, &buf);
    nu_path_free(path);
    if (result != NU_SUCCESS) {
        nu_interrupt(NUVK_LOGGER_NAME"Failed to read include file: %s.\n", requested_source);
    }

    include_result->content_length     = nbytes;
    include_result->content            = buf;
    include_result->source_name        = requested_source;
    include_result->source_name_length = strlen(requested_source);

    return include_result;
}
static void releaser(
    void *user,
    shaderc_include_result *result
)
{
    nu_free((char*)result->content);
    nu_free(result);
}

nu_result_t nuvk_shader_manager_initialize(nuvk_shader_manager_t *manager)
{
    manager->compiler = shaderc_compiler_initialize();
    manager->options  = shaderc_compile_options_initialize();

    shaderc_compile_options_set_target_env(manager->options, shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);
    shaderc_compile_options_set_target_spirv(manager->options, shaderc_spirv_version_1_5);
    shaderc_compile_options_set_optimization_level(manager->options, shaderc_optimization_level_performance);
    if (NUVK_SHADER_INCLUDE_DEBUG_SYMBOLS) {
        shaderc_compile_options_set_generate_debug_info(manager->options);
    }
    shaderc_compile_options_set_include_callbacks(manager->options, resolver, releaser, NULL);

    return NU_SUCCESS;
}
nu_result_t nuvk_shader_manager_terminate(nuvk_shader_manager_t *manager)
{
    shaderc_compiler_release(manager->compiler);
    shaderc_compile_options_release(manager->options);

    return NU_SUCCESS;
}

static shaderc_shader_kind vulkan_stage_to_shaderc_kind(VkShaderStageFlags stage)
{
    if (stage == VK_SHADER_STAGE_VERTEX_BIT) {
        return shaderc_vertex_shader;
    } else if (stage == VK_SHADER_STAGE_FRAGMENT_BIT) {
        return shaderc_fragment_shader;
    } else if (stage == VK_SHADER_STAGE_COMPUTE_BIT) {
        return shaderc_compute_shader;
    }
    nu_warning(NUVK_LOGGER_NAME"Unknown shader stage flag. Using vertex shader...\n");
    return shaderc_vertex_shader;
}

static nu_result_t glsl_to_spirv(
    const nuvk_shader_manager_t *manager,
    const char *path, 
    VkShaderStageFlags stage,
    uint32_t *code_size,
    uint32_t **code
)
{
    /* read glsl file */
    uint32_t nbytes;
    char *buf;
    if (nu_file_read_allbytes(path, &nbytes, &buf) != NU_SUCCESS) {
        nu_error(NUVK_LOGGER_NAME"Failed to read file: %s.\n", path);
        return NU_FAILURE;
    }

    /* compile to spirv */
    shaderc_compilation_result_t compilation_result = shaderc_compile_into_spv(manager->compiler, buf,
        nbytes, vulkan_stage_to_shaderc_kind(stage), path, "main", manager->options);
    nu_free(buf);
    if (compilation_result == NULL) {
        nu_error(NUVK_LOGGER_NAME"Failed to compile glsl to spirv (internal error).\n");
        return NU_FAILURE;
    } else if (shaderc_result_get_compilation_status(compilation_result) != shaderc_compilation_status_success) {
        nu_error(NUVK_LOGGER_NAME"Failed to compile glsl to spirv :\n");
        nu_error("%s", shaderc_result_get_error_message(compilation_result));
        shaderc_result_release(compilation_result);
        return NU_FAILURE;
    }

    /* recover spirv code */
    *code_size = shaderc_result_get_length(compilation_result);
    *code      = (uint32_t*)nu_malloc(*code_size);
    memcpy(*code, shaderc_result_get_bytes(compilation_result), *code_size);
    shaderc_result_release(compilation_result);

    return NU_SUCCESS;
}

nu_result_t nuvk_shader_module_create_from_glsl(
    const nuvk_context_t *context,
    const nuvk_shader_manager_t *manager,
    const char *filename,
    VkShaderStageFlags stage,
    VkShaderModule *module
)
{
    nu_path_t path;
    nu_path_allocate_cstr(filename, &path);

    /* convert glsl to spirv */
    uint32_t code_size;
    uint32_t *code;
    nu_result_t result = glsl_to_spirv(manager, nu_path_get_cstr(path), stage, &code_size, &code);
    nu_path_free(path);
    if (result != NU_SUCCESS) {
        nu_error(NUVK_LOGGER_NAME"Failed to convert glsl to spirv: %s.\n", filename);
        return NU_FAILURE;
    }

    /* create shader module */
    VkShaderModuleCreateInfo info;
    memset(&info, 0, sizeof(VkShaderModuleCreateInfo));
    info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    info.codeSize = code_size;
    info.pCode    = code;
    
    VkResult res = vkCreateShaderModule(context->device, &info, &context->allocator, module);
    nu_free(code);
    if (res != VK_SUCCESS) {
        nu_error("Failed to create shader module: %s.\n", filename);
        return NU_FAILURE;
    }

    return NU_SUCCESS;
}