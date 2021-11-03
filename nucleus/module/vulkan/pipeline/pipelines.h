#ifndef NUVK_PIPELINES_H
#define NUVK_PIPELINES_H

#include <nucleus/module/vulkan/pipeline/generator.h>
#include <nucleus/module/vulkan/pipeline/geometry.h>
#include <nucleus/module/vulkan/pipeline/light.h>
#include <nucleus/module/vulkan/pipeline/postprocess.h>

typedef struct {
    nuvk_pipeline_generator_t generator;
    nuvk_pipeline_geometry_t geometry;
    nuvk_pipeline_light_t light;
    nuvk_pipeline_postprocess_t postprocess;
} nuvk_pipelines_t;

#endif