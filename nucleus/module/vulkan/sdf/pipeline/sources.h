#ifndef NUVK_SDF_PIPELINE_SOURCES_H
#define NUVK_SDF_PIPELINE_SOURCES_H

#include <nucleus/nucleus.h>

typedef enum {
    NUVK_SDF_PIPELINE_SOURCE_CONSTANTS_GLSL         = 0,
    NUVK_SDF_PIPELINE_SOURCE_GEOMETRY_TEMPLATE_FRAG = 1,
    NUVK_SDF_PIPELINE_SOURCE_GEOMETRY_TEMPLATE_VERT = 2,
    NUVK_SDF_PIPELINE_SOURCE_POSTPROCESS_FRAG       = 3,
    NUVK_SDF_PIPELINE_SOURCE_POSTPROCESS_VERT       = 4,
    NUVK_SDF_PIPELINE_SOURCE_COUNT                  = 5
} nuvk_sdf_pipeline_sources_name_t;

#define NUVK_SDF_PIPELINE_INJECT_CONSTANTS "__INJECT_CONSTANTS__"
#define NUVK_SDF_PIPELINE_INJECT_INSTANCES "__INJECT_INSTANCES__"

nu_result_t nuvk_sdf_pipeline_sources_load(nu_string_t *sources);
nu_result_t nuvk_sdf_pipeline_sources_unload(nu_string_t *sources);

#endif