#ifndef NUVK_ASSETS_INTERFACE_H
#define NUVK_ASSETS_INTERFACE_H

#include <nucleus/nucleus.h>

#define NUVK_MAX_MATERIAL_COUNT 128
#define NUVK_MAX_SDF_COUNT      32

NU_DECLARE_HANDLE(nuvk_sdf_t);
NU_DECLARE_HANDLE(nuvk_material_t);

typedef float (*nuvk_sdf_pfn_t)(const nu_vec3f_t, const void*);
typedef void (*nuvk_sdf_aabb_pfn_t)(const void*, nu_aabb_t*);

typedef struct {
    uint32_t data_size;
    const char *glsl_data_code;
    const char *glsl_sdf_code;
    nuvk_sdf_pfn_t c_sdf_code;
    nuvk_sdf_aabb_pfn_t c_aabb_code;
    uint32_t max_instance_count;
} nuvk_sdf_info_t;

typedef struct {
    nu_vec3f_t color;
} nuvk_material_info_t;

#endif