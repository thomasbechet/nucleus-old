#include <nucleus/module/vulkan/pipeline/generator.h>

#include <nucleus/module/vulkan/core/context.h>

static const char *glsl_sources[] = {
    "$ENGINE_DIR/shader/geometry_template.frag",
    "$ENGINE_DIR/shader/geometry_template.vert",
    "$ENGINE_DIR/shader/light.comp",
    "$ENGINE_DIR/shader/postprocess.frag",
    "$ENGINE_DIR/shader/postprocess.vert"
};

static nu_result_t nuvk_pipeline_sources_load(nu_string_t *sources)
{
    nu_string_t path;
    nu_string_allocate(&path);

    for (uint32_t i = 0; i < NUVK_PIPELINE_SOURCE_COUNT; i++) {
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

static nu_result_t nuvk_pipeline_generate_instance_source(
    const nuvk_sdf_data_t **sdfs,
    uint32_t sdf_count,
    nu_string_t *source
)
{
    /* clear source */
    nu_string_clear(source);

    /* constants */
    if (sdf_count > 0) {
        nu_string_append_cstr(source, "#define NO_INSTANCE_TYPES true\n");
    }

    /* append instance definition */
    for (uint32_t i = 0; i < sdf_count; i++) {
        nu_string_t instance_definition;
        nu_string_allocate_format(&instance_definition,
            "struct InstanceData%ld {\n"
            "%s\n"
            "};\n"
            "struct Instance%ld {\n"
            "	mat3 invRotation;\n"
            "	vec4 translationScale;\n"
            "   uint materialIndex;\n"
            "	InstanceData%ld data;\n"
            "};\n"
            "float sdf%ld(in vec3 p, in InstanceData%ld data) {\n"
            "   %s\n"
            "}\n",
            i, sdfs[i]->glsl_data_code, i, i, i, i, sdfs[i]->glsl_sdf_code);
        nu_string_append(source, instance_definition);
        nu_string_free(instance_definition);
    }

    /* append index ubo */
    nu_string_append_cstr(source, "layout(set = 0, binding = 1, std140) uniform IndexUBO {\n");
    for (uint32_t i = 0; i < sdf_count; i++) {
        nu_string_t instance_block;
        nu_string_allocate_format(&instance_block,
            "   uint indices%ld[%ld];\n",
            i, sdfs[i]->max_instance_count);
        nu_string_append(source, instance_block);
        nu_free(instance_block);
    }
    if (sdf_count == 0) {
        nu_string_append_cstr(source, "  uint unusedIndex;\n");
    }
    nu_string_append_cstr(source, "};\n");

    /* append instance ubo */
    nu_string_append_cstr(source, "layout(set = 0, binding = 2, std140) uniform InstanceUBO {\n");
    for (uint32_t i = 0; i < sdf_count; i++) {
        nu_string_t instance_line;
        nu_string_allocate_format(&instance_line,
            "   Instance%ld instances%ld[%ld];\n",
            i, i, sdfs[i]->max_instance_count);
        nu_string_append(source, instance_line);
        nu_string_free(instance_line);
    }
    if (sdf_count == 0) {
        nu_string_append_cstr(source, "  uint unusedInstance;\n");
    }
    nu_string_append_cstr(source, "};\n");

    /* trace primary function */
    nu_string_append_cstr(source,
        "float tracePrimary(in vec3 pos, in vec3 dir, in float radiusFactor, out vec3 normal, out uint materialIndex) {\n"
        "   normal = dir;\n"
        "   float hitDepth = MAX_DISTANCE;\n");
    for (uint32_t i = 0; i < sdf_count; i++) {
        nu_string_t instance_code;
        nu_string_allocate_format(&instance_code,
            "   for (uint i = 0; i < %ld; i++) {\n"
            "       uint index = indices%ld[i];\n"
            "       if (index == 0xFFFFFFFF) break;\n"
            "       vec3 relPos = instances%ld[index].invRotation * (pos - instances%ld[index].translationScale.xyz);\n"
            "       vec3 relDir = normalize(instances%ld[index].invRotation * dir);\n"
            "       float s     = instances%ld[index].translationScale.w;\n"
            "       InstanceData%ld data = instances%ld[index].data;\n"
            "       float depth = 0.0;\n"
            "       for (uint step = 0; depth < hitDepth && step < MAX_RAYMARCH_STEP; step++) {\n"
            "           vec3 p = relPos + depth * relDir;\n"
            "           float sd = sdf%ld(p / s, data) * s;\n"
            "           if (sd < depth * radiusFactor) {\n"
            "               const vec2 e = vec2(EPSILON, 0);\n"
            "               normal = normalize(vec3(\n"
            "                   sd - sdf%ld(vec3(p - e.xyy) / s, data) * s,\n"
            "                   sd - sdf%ld(vec3(p - e.yxy) / s, data) * s,\n"
            "                   sd - sdf%ld(vec3(p - e.yyx) / s, data) * s\n"
            "               ));\n"
            "               normal = transpose(instances%ld[index].invRotation) * normal;\n"
            "               materialIndex = instances%ld[index].materialIndex;\n"
            "               hitDepth = depth;\n"
            "               break;\n"
            "           }\n"
            "           depth += sd;\n"
            "       }\n"
            "    }\n",
            sdfs[i]->max_instance_count, i, i, i, i, i, i, i, i, i, i, i, i, i);
        nu_string_append(source, instance_code);
        nu_string_free(instance_code);
    }
    nu_string_append_cstr(source,
        "   return hitDepth;\n"
        "}\n");

    /* trace function */
    nu_string_append_cstr(source,
        "float trace(in vec3 pos, in vec3 dir, out vec3 normal) {\n"
        "   normal = dir;\n"
        "   float hitDepth = MAX_DISTANCE;\n");
    for (uint32_t i = 0; i < sdf_count; i++) {
        nu_string_t instance_code;
        nu_string_allocate_format(&instance_code,
            "   for (uint i = 0; i < %ld; i++) {\n"
            "       uint index = indices%ld[i];\n"
            "       if (index == 0xFFFFFFFF) break;\n"
            "       vec3 relPos = instances%ld[index].invRotation * (pos - instances%ld[index].translationScale.xyz);\n"
            "       vec3 relDir = normalize(instances%ld[index].invRotation * dir);\n"
            "       float s     = instances%ld[index].translationScale.w;\n"
            "       InstanceData%ld data = instances%ld[index].data;\n"
            "       float depth = 0.0;\n"
            "       for (int step = 0; depth < hitDepth && step < MAX_RAYMARCH_STEP; step++) {\n"
            "           vec3 p = relPos + depth * relDir;\n"
            "           float sd = sdf%ld(p / s, data) * s;\n"
            "           if (sd < MIN_HIT_DISTANCE) {\n"
            "               const vec2 e = vec2(EPSILON, 0);\n"
            "               normal = normalize(vec3(\n"
            "                   sd - sdf%ld(vec3(p - e.xyy) / s, data) * s,\n"
            "                   sd - sdf%ld(vec3(p - e.yxy) / s, data) * s,\n"
            "                   sd - sdf%ld(vec3(p - e.yyx) / s, data) * s\n"
            "               ));\n"
            "               normal = transpose(instances%ld[index].invRotation) * normal;\n"
            "               hitDepth = depth;\n"
            "               break;\n"
            "           }\n"
            "           depth += sd;\n"
            "       }\n"
            "    }\n",
            sdfs[i]->max_instance_count, i, i, i, i, i, i, i, i, i, i, i, i);
        nu_string_append(source, instance_code);
        nu_string_free(instance_code);
    }
    nu_string_append_cstr(source,
        "   return hitDepth;\n"
        "}\n");

    return NU_SUCCESS;
}
static nu_result_t nuvk_pipeline_generate_materials_source(nu_string_t *source)
{
    /* clear source */
    nu_string_clear(source);

    /* generate source code */
    nu_string_t material_definition;
    nu_string_allocate_format(&material_definition,
        "struct MaterialData {\n"
        "   vec4 color;\n"
        "};\n"
        "layout(set = 0, binding = 3) uniform MaterialsUBO {\n"
        "   MaterialData materials[%d];\n"
        "};\n",
        NUVK_MAX_MATERIAL_COUNT
    );
    nu_string_append(source, material_definition);
    nu_string_free(material_definition);

    return NU_SUCCESS;
}
static nu_result_t nuvk_pipeline_generate_environment_source(nu_string_t *source)
{
    nu_string_clear(source);
    nu_string_append_cstr(source,
        "layout(set = 0, binding = 0) uniform EnvironmentUBO {\n"
        "    mat4 VPMatrix;\n"
        "    vec3 eye;\n"
        "    float pixelRadiusFactor;\n"
        "};\n"
    );
    return NU_SUCCESS;
}
static nu_result_t nuvk_pipeline_generate_constants_source(nu_string_t *source)
{
    nu_string_clear(source);
    nu_string_append_cstr(source,
        "#define MAX_RAYMARCH_STEP 512\n"
        "#define MAX_DISTANCE      5000.0\n"
        "#define MIN_HIT_DISTANCE  0.0005\n"
        "#define EPSILON           0.001\n"
    );
    return NU_SUCCESS;
}

nu_result_t nuvk_pipeline_generator_initialize(nuvk_pipeline_generator_t *generator)
{
    /* load sources */
    nu_result_t result = nuvk_pipeline_sources_load(generator->sources);
    NU_CHECK(result == NU_SUCCESS, return result, NUVK_LOGGER_NAME, "Failed to load sources.");

    /* allocate injections */
    nu_string_allocate(&generator->inject_constants);
    nu_string_allocate(&generator->inject_environment);
    nu_string_allocate(&generator->inject_instances);
    nu_string_allocate(&generator->inject_materials);

    /* initial injection code */
    nuvk_pipeline_generate_constants_source(&generator->inject_constants);
    nuvk_pipeline_generate_environment_source(&generator->inject_environment);
    nuvk_pipeline_generate_instance_source(NULL, 0, &generator->inject_instances);
    nuvk_pipeline_generate_materials_source(&generator->inject_materials);

    return NU_SUCCESS;
}
nu_result_t nuvk_pipeline_generator_terminate(nuvk_pipeline_generator_t *generator)
{
    /* free sources */
    for (uint32_t i = 0; i < NUVK_PIPELINE_SOURCE_COUNT; i++) {
        if (generator->sources[i] != NU_NULL_HANDLE) {
            nu_string_free(generator->sources[i]);
        }
    }

    /* free injections */
    nu_string_free(generator->inject_constants);
    nu_string_free(generator->inject_environment);
    nu_string_free(generator->inject_instances);
    nu_string_free(generator->inject_materials);

    return NU_SUCCESS;
}
nu_result_t nuvk_pipeline_generator_update_sdfs(
    nuvk_pipeline_generator_t *generator,
    const nuvk_sdf_data_t **sdfs,
    uint32_t sdf_count
)
{
    nuvk_pipeline_generate_instance_source(sdfs, sdf_count, &generator->inject_instances);
    return NU_SUCCESS;
}

nu_result_t nuvk_pipeline_generator_get_source(
    const nuvk_pipeline_generator_t *generator,
    nuvk_pipeline_source_name_t name,
    nu_string_t *source
)
{
    nu_string_clear(source);
    switch (name)
    {
    case NUVK_PIPELINE_SOURCE_GEOMETRY_TEMPLATE_FRAG:
        nu_string_set(source, generator->sources[name]);
        nu_string_replace(source, NUVK_PIPELINE_INJECT_CONSTANTS, generator->inject_constants);
        nu_string_replace(source, NUVK_PIPELINE_INJECT_INSTANCES, generator->inject_instances);
        nu_string_replace(source, NUVK_PIPELINE_INJECT_ENVIRONMENT, generator->inject_environment);
        nu_string_replace(source, NUVK_PIPELINE_INJECT_MATERIALS, generator->inject_materials);
        break;
    case NUVK_PIPELINE_SOURCE_GEOMETRY_TEMPLATE_VERT:
        nu_string_set(source, generator->sources[name]);
        nu_string_replace(source, NUVK_PIPELINE_INJECT_ENVIRONMENT, generator->inject_environment);
        break;
    case NUVK_PIPELINE_SOURCE_LIGHT_COMP:
        nu_string_set(source, generator->sources[name]);
        nu_string_replace(source, NUVK_PIPELINE_INJECT_CONSTANTS, generator->inject_constants);
        nu_string_replace(source, NUVK_PIPELINE_INJECT_INSTANCES, generator->inject_instances);
        nu_string_replace(source, NUVK_PIPELINE_INJECT_ENVIRONMENT, generator->inject_environment);
        nu_string_replace(source, NUVK_PIPELINE_INJECT_MATERIALS, generator->inject_materials);
        break;
    case NUVK_PIPELINE_SOURCE_POSTPROCESS_FRAG:
        nu_string_set(source, generator->sources[name]);
        nu_string_replace(source, NUVK_PIPELINE_INJECT_INSTANCES, generator->inject_instances);
        nu_string_replace(source, NUVK_PIPELINE_INJECT_ENVIRONMENT, generator->inject_environment);
        nu_string_replace(source, NUVK_PIPELINE_INJECT_CONSTANTS, generator->inject_constants);
        break;
    case NUVK_PIPELINE_SOURCE_POSTPROCESS_VERT:
        nu_string_set(source, generator->sources[name]);
        nu_string_replace(source, NUVK_PIPELINE_INJECT_ENVIRONMENT, generator->inject_environment);
        break;
    default:
        nu_warning(NUVK_LOGGER_NAME, "Unknown source.");
        break;
    }

    return NU_SUCCESS;
}