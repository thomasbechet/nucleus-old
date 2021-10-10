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

nu_result_t nuvk_sdf_pipeline_generate_instance_source(
    const nuvk_sdf_instance_type_info_t *types,
    uint32_t type_count,
    nu_string_t *source
)
{
    /* clear source */
    nu_string_clear(source);

    /* constants */
    if (type_count > 0) {
        nu_string_append_cstr(source, "#define NO_INSTANCE_TYPES true\n");
    }

    /* append instance definition */
    for (uint32_t i = 0; i < type_count; i++) {
        nu_string_t instance_definition;
        nu_string_allocate_format(&instance_definition,
            "struct InstanceData%ld {\n"
            "%s\n"
            "};\n"
            "struct Instance%ld {\n"
            "	mat3 invRotation;\n"
            "	vec4 translationScale;\n"
            "	InstanceData%ld data;\n"
            "};\n"
            "float sdf%ld(in vec3 p, in InstanceData%ld data) {\n"
            "   %s\n"
            "}\n",
            i, types[i].glsl_data_code, i, i, i, i, types[i].glsl_sdf_code);
        nu_string_append(source, instance_definition);
        nu_string_free(instance_definition);
    }

    /* append index ubo */
    nu_string_append_cstr(source, "layout(set = 0, binding = 1, std140) uniform IndexUBO {\n");
    for (uint32_t i = 0; i < type_count; i++) {
        nu_string_t instance_block;
        nu_string_allocate_format(&instance_block,
            "   uint indices%ld[%ld];\n",
            i, types[i].max_instance_count);
        nu_string_append(source, instance_block);
        nu_free(instance_block);
    }
    if (type_count == 0) {
        nu_string_append_cstr(source, "  uint unusedIndex;\n");
    }
    nu_string_append_cstr(source, "};\n");

    /* append instance ubo */
    nu_string_append_cstr(source, "layout(set = 0, binding = 2, std140) uniform InstanceUBO {\n");
    for (uint32_t i = 0; i < type_count; i++) {
        nu_string_t instance_line;
        nu_string_allocate_format(&instance_line,
            "   Instance%ld instances%ld[%ld];\n",
            i, i, types[i].max_instance_count);
        nu_string_append(source, instance_line);
        nu_string_free(instance_line);
    }
    if (type_count == 0) {
        nu_string_append_cstr(source, "  uint unusedInstance;\n");
    }
    nu_string_append_cstr(source, "};\n");

    /* trace primary function */
    nu_string_append_cstr(source,
        "float tracePrimary(in vec3 pos, in vec3 dir, in float radiusFactor, out vec3 normal) {\n"
        "   normal = dir;\n"
        "   float hitDepth = MAX_DISTANCE;\n");
    for (uint32_t i = 0; i < type_count; i++) {
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
            "               hitDepth = depth;\n"
            "               break;\n"
            "           }\n"
            "           depth += sd;\n"
            "       }\n"
            "    }\n",
            types[i].max_instance_count, i, i, i, i, i, i, i, i, i, i, i, i);
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
    for (uint32_t i = 0; i < type_count; i++) {
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
            types[i].max_instance_count, i, i, i, i, i, i, i, i, i, i, i, i);
        nu_string_append(source, instance_code);
        nu_string_free(instance_code);
    }
    nu_string_append_cstr(source,
        "   return hitDepth;\n"
        "}\n");

    return NU_SUCCESS;
}