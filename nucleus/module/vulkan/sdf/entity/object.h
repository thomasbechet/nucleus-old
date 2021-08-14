#ifndef NUVK_SDF_OBJECT_H
#define NUVK_SDF_OBJECT_H

#include <nucleus/nucleus.h>

typedef struct {
    nu_transform_t transform;
    void *data;
} nuvk_sdf_object_t;

typedef float (*nuvk_sdf_pfn_t)(nu_vec3f_t, const void*);

typedef struct {
    const char *glsl_sdf;
    nuvk_sdf_pfn_t c_sdf;
    uint32_t data_size;
} nuvk_sdf_type_t;

typedef struct {
    const char *glsl_sdf;
    nuvk_sdf_pfn_t c_sdf;
    uint32_t data_size;
} nuvk_sdf_type_info_t;

#endif