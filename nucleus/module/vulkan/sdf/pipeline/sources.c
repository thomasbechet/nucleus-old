#include <nucleus/module/vulkan/sdf/pipeline/sources.h>

#include <nucleus/module/vulkan/core/context.h>

const char *glsl_sources[] = {
    "$ENGINE_DIR/shader/constants.glsl",
    "$ENGINE_DIR/shader/geometry_template.frag",
    "$ENGINE_DIR/shader/geometry_template.vert",
    "$ENGINE_DIR/shader/light.comp",
    "$ENGINE_DIR/shader/postprocess.frag",
    "$ENGINE_DIR/shader/postprocess.vert"
};

nu_result_t nuvk_sdf_pipeline_sources_load(nu_string_t *sources)
{
    nu_string_t path;
    nu_string_allocate(&path);

    for (uint32_t i = 0; i < NUVK_SDF_PIPELINE_SOURCE_COUNT; i++) {
        nu_string_set_cstr(&path, glsl_sources[i]);
        nu_string_resolve_path(&path);
        if (nu_io_readall_string(nu_string_get_cstr(path), &sources[i]) != NU_SUCCESS) {
            sources[i] = NU_NULL_HANDLE;
            nu_warning(NUVK_LOGGER_NAME, "Failed to read source: %s.", glsl_sources[i]);
        }
    }

    nu_string_free(path);

    return NU_SUCCESS;
}
nu_result_t nuvk_sdf_pipeline_sources_unload(nu_string_t *sources)
{
    for (uint32_t i = 0; i < NUVK_SDF_PIPELINE_SOURCE_COUNT; i++) {
        if (sources[i] != NU_NULL_HANDLE) {
            nu_string_free(sources[i]);
        }
    }

    return NU_SUCCESS;
}