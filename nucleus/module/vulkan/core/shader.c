#include <nucleus/module/vulkan/core/shader.h>

#include <shaderc/shaderc.h>

// static shaderc_include_result *resolver(
//     void *user,
//     const char *requested_source,
//     int type,
//     const char *requesting_source,
//     size_t include_depth
// )
// {
//     shaderc_include_result *include_result = (shaderc_include_result*)nu_malloc(sizeof(shaderc_include_result));

//     nu_path_t path;
//     nu_path_allocate_cstr(requested_source, &path);

//     include_result->source_name_length = nu_path_get_length(path);
//     include_result->source_name        = (const char*)nu_malloc(sizeof(char) * include_result->source_name_length + 1);
//     memcpy((char*)include_result->source_name, nu_path_get_cstr(path), include_result->source_name_length);
//     ((char*)include_result->source_name)[include_result->source_name_length] = '\0';

//     nu_path_free(path);

//     uint32_t nbytes;
//     char *buf;
//     nu_result_t result = nu_file_readall_bytes(include_result->source_name, &nbytes, &buf);
//     if (result != NU_SUCCESS) {
//         nu_interrupt(NUVK_LOGGER_NAME"Failed to read include file: %s.\n", requested_source);
//     }

//     include_result->content_length = nbytes;
//     include_result->content        = buf;

//     return include_result;
// }
// static void releaser(
//     void *user,
//     shaderc_include_result *result
// )
// {
//     nu_free((char*)result->content);
//     nu_free((char*)result->source_name);
//     nu_free(result);
// }

nu_result_t nuvk_shader_manager_initialize(nuvk_shader_manager_t *manager)
{
    /* initialize shaderc compiler */
    manager->compiler = shaderc_compiler_initialize();
    manager->options  = shaderc_compile_options_initialize();

    shaderc_compile_options_set_target_env(manager->options, shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);
    shaderc_compile_options_set_target_spirv(manager->options, shaderc_spirv_version_1_5);
    shaderc_compile_options_set_optimization_level(manager->options, shaderc_optimization_level_performance);
    if (NUVK_SHADER_INCLUDE_DEBUG_SYMBOLS) {
        shaderc_compile_options_set_generate_debug_info(manager->options);
    }
    // shaderc_compile_options_set_include_callbacks(manager->options, resolver, releaser, NULL);

    /* allocate resources */
    nu_array_allocate(&manager->injectors, sizeof(nu_string_t));
    nu_string_allocate(&manager->placeholder_source);

    return NU_SUCCESS;
}
nu_result_t nuvk_shader_manager_terminate(nuvk_shader_manager_t *manager)
{
    /* free resources */
    nu_string_t *injectors  = (nu_string_t*)nu_array_get_data(manager->injectors);
    uint32_t injector_count = nu_array_get_size(manager->injectors);
    for (uint32_t i = 0; i < injector_count; i++) {
        nu_string_free(injectors[i]);
    }
    nu_array_free(manager->injectors);
    nu_string_free(manager->placeholder_source);

    /* free compiler */
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
    nu_warning(NUVK_LOGGER_NAME, "Unknown shader stage flag. Using vertex shader...");
    return shaderc_vertex_shader;
}

static nu_result_t glsl_source_to_spirv_code(
    const nuvk_shader_manager_t *manager,
    VkShaderStageFlags stage,
    nu_string_t glsl_source,
    const char *identifier,
    uint32_t *code_size,
    uint32_t **code
)
{
    nu_path_t filename = NU_NULL_HANDLE;

    if (NUVK_SHADER_DUMP_SOURCES) {        
        nu_file_t file;
        nu_result_t result;

        nu_path_allocate_format(&filename, "$ENGINE_DIR/shader/dump/%s.dump", identifier);
        result = nu_file_open(filename, NU_IO_MODE_WRITE, &file);
        if (result == NU_SUCCESS) {
            if (nu_file_write_string(file, glsl_source) != NU_SUCCESS) {
                nu_warning(NUVK_LOGGER_NAME, "Failed to write shader source: %s.", nu_path_get_cstr(filename));
            }
            nu_file_close(file);
        } else {
            nu_warning(NUVK_LOGGER_NAME, "Failed to open file to write shader cache: %s.", nu_path_get_cstr(filename));
        }
    }

    /* compile to spirv */
    const char *input_filename = (filename == NU_NULL_HANDLE) ? "" : nu_path_get_cstr(filename);
    shaderc_compilation_result_t compilation_result;
    compilation_result = shaderc_compile_into_spv(manager->compiler, nu_string_get_cstr(glsl_source),
        nu_string_get_length(glsl_source), vulkan_stage_to_shaderc_kind(stage), input_filename, "main", manager->options);

    if (NUVK_SHADER_DUMP_SOURCES) {
        nu_path_free(filename);
    }

    /* check errors */
    NU_CHECK(compilation_result != NULL, return NU_FAILURE, NUVK_LOGGER_NAME, "Failed to compile glsl source to spirv code (internal error).");
    NU_CHECK(shaderc_result_get_compilation_status(compilation_result) == shaderc_compilation_status_success, goto cleanup0, NUVK_LOGGER_NAME, "Failed to compile glsl source to spirv code.");

    /* recover spirv code */
    *code_size = shaderc_result_get_length(compilation_result);
    *code      = (uint32_t*)nu_malloc(*code_size);
    memcpy(*code, shaderc_result_get_bytes(compilation_result), *code_size);
    shaderc_result_release(compilation_result);

    return NU_SUCCESS;

cleanup0: ;
    nu_string_t error_message;
    nu_string_array_t error_lines;
    nu_string_array_allocate(&error_lines);
    nu_string_allocate_cstr(&error_message, shaderc_result_get_error_message(compilation_result));
    nu_string_split(error_message, "\n", error_lines);
    for (uint32_t i = 0; i < nu_string_array_get_length(error_lines); i++) {
        nu_error(NUVK_LOGGER_NAME, nu_string_array_get(error_lines, i));
    }
    nu_string_array_free(error_lines);
    nu_string_free(error_message);

    shaderc_result_release(compilation_result);

    return NU_FAILURE;
}

nu_result_t nuvk_shader_module_create_from_glsl_source(
    const nuvk_context_t *context,
    const nuvk_shader_manager_t *manager,
    VkShaderStageFlags stage,
    nu_string_t glsl_source,
    const char *identifier,
    VkShaderModule *module
)
{
    /* convert glsl source to spirv code */
    uint32_t code_size;
    uint32_t *code;
    nu_result_t result = glsl_source_to_spirv_code(manager, stage, glsl_source, identifier, &code_size, &code);
    NU_CHECK(result == NU_SUCCESS, return NU_FAILURE, NUVK_LOGGER_NAME, "Failed to convert glsl source to spirv code.");

    /* create shader module */
    VkShaderModuleCreateInfo info;
    memset(&info, 0, sizeof(VkShaderModuleCreateInfo));
    info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    info.codeSize = code_size;
    info.pCode    = code;
    
    VkResult res = vkCreateShaderModule(context->device, &info, &context->allocator, module);
    nu_free(code);
    NU_CHECK(res == VK_SUCCESS, return NU_FAILURE, NUVK_LOGGER_NAME, "Failed to create shader module.");

    return NU_SUCCESS;
}