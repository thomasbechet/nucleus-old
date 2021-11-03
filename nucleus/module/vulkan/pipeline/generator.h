#ifndef NUVK_PIPELINE_GENERATOR_H
#define NUVK_PIPELINE_GENERATOR_H

#include <nucleus/module/vulkan/assets/sdf.h>

typedef enum {
    NUVK_PIPELINE_SOURCE_GEOMETRY_TEMPLATE_FRAG = 0,
    NUVK_PIPELINE_SOURCE_GEOMETRY_TEMPLATE_VERT = 1,
    NUVK_PIPELINE_SOURCE_LIGHT_COMP             = 2,
    NUVK_PIPELINE_SOURCE_POSTPROCESS_FRAG       = 3,
    NUVK_PIPELINE_SOURCE_POSTPROCESS_VERT       = 4,
    NUVK_PIPELINE_SOURCE_COUNT                  = 5
} nuvk_pipeline_source_name_t;

#define NUVK_PIPELINE_INJECT_CONSTANTS   "__INJECT_CONSTANTS__"
#define NUVK_PIPELINE_INJECT_INSTANCES   "__INJECT_INSTANCES__"
#define NUVK_PIPELINE_INJECT_MATERIALS   "__INJECT_MATERIALS__"
#define NUVK_PIPELINE_INJECT_ENVIRONMENT "__INJECT_ENVIRONMENT__"

typedef struct {
    nu_string_t sources[NUVK_PIPELINE_SOURCE_COUNT];
    nu_string_t inject_constants;
    nu_string_t inject_instances;
    nu_string_t inject_materials;
    nu_string_t inject_environment;
} nuvk_pipeline_generator_t;

nu_result_t nuvk_pipeline_generator_initialize(nuvk_pipeline_generator_t *generator);
nu_result_t nuvk_pipeline_generator_terminate(nuvk_pipeline_generator_t *generator);
nu_result_t nuvk_pipeline_generator_update_sdfs(
    nuvk_pipeline_generator_t *generator,
    const nuvk_sdf_data_t **sdfs,
    uint32_t sdf_count
);

nu_result_t nuvk_pipeline_generator_get_source(
    const nuvk_pipeline_generator_t *generator,
    nuvk_pipeline_source_name_t name,
    nu_string_t *source
);

#endif