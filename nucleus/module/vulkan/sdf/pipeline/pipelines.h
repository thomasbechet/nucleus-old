#ifndef NUVK_SDF_PIPELINES_H
#define NUVK_SDF_PIPELINES_H

#include <nucleus/module/vulkan/sdf/pipeline/sources.h>
#include <nucleus/module/vulkan/sdf/pipeline/geometry.h>
#include <nucleus/module/vulkan/sdf/pipeline/light.h>
#include <nucleus/module/vulkan/sdf/pipeline/postprocess.h>

typedef struct {
    nu_string_t sources[NUVK_SDF_PIPELINE_SOURCE_COUNT];
    nuvk_sdf_pipeline_geometry_t geometry;
    nuvk_sdf_pipeline_light_t light;
    nuvk_sdf_pipeline_postprocess_t postprocess;
} nuvk_sdf_pipelines_t;

#endif