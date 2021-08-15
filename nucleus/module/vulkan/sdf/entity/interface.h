#ifndef NUVK_SDF_ENTITY_INTERFACE_H
#define NUVK_SDF_ENTITY_INTERFACE_H

#include <nucleus/nucleus.h>

NU_DECLARE_HANDLE(nuvk_sdf_instance_type_t);
NU_DECLARE_HANDLE(nuvk_sdf_instance_t);

typedef float (*nuvk_sdf_instance_sdf_pfn_t)(nu_vec3f_t, const void*);
typedef void (*nuvk_sdf_instance_aabb_pfn_t)(const void*, nu_aabb_t*);

typedef struct {
    uint32_t data_size;
    const char *glsl_data_code;
    const char *glsl_sdf_code;
    nuvk_sdf_instance_sdf_pfn_t c_sdf_code;
    nuvk_sdf_instance_aabb_pfn_t c_aabb_code;
} nuvk_sdf_instance_type_info_t;

typedef enum {
    NUVK_SDF_INSTANCE_FLAG_STATIC = 1
} nuvk_sdf_instance_flags_t;

typedef struct {
    nu_transform_t transform;
    nuvk_sdf_instance_type_t type;
    nuvk_sdf_instance_flags_t flags;
    void *data;
} nuvk_sdf_instance_info_t;

#endif