#ifndef NUVK_SCENE_INTERFACE_H
#define NUVK_SCENE_INTERFACE_H

#include <nucleus/module/vulkan/assets/interface.h>

NU_DECLARE_HANDLE(nuvk_sdf_instance_t);

// typedef enum {
//     NUVK_SDF_INSTANCE_FLAG_STATIC_TRANSFORM = 1 << 0,
//     NUVK_SDF_INSTANCE_FLAG_STATIC_DATA      = 1 << 1,
//     NUVK_SDF_INSTANCE_FLAG_STATIC_AABB      = 1 << 2 
// } nuvk_sdf_instance_flags_t;

typedef struct {
    nu_vec3f_t translation;
    nu_quatf_t rotation;
    float scale;
} nuvk_sdf_transform_t;

typedef struct {
    nuvk_sdf_transform_t transform;
    nuvk_sdf_t sdf;
    nuvk_material_t material;
    void *data;
} nuvk_sdf_instance_info_t;

/* primitives */

typedef struct {
    float radius;
    float _pad0[3];
} nuvk_sdf_sphere_data_t;

typedef struct {
    nu_vec3f_t box;
    float radius;
} nuvk_sdf_cube_data_t;

typedef struct {
    float _pad0[4];
} nuvk_sdf_menger_sponge_data_t;

typedef struct {
    float x;
    float y;
    float _pad0[2];
} nuvk_sdf_torus_data_t;

typedef struct {
    nu_vec3f_t normal;
    float height;
} nuvk_sdf_plane_data_t;

typedef enum {
    NUVK_SDF_PRIMITIVE_SPHERE        = 0,
    NUVK_SDF_PRIMITIVE_CUBE          = 1,
    NUVK_SDF_PRIMITIVE_MENGER_SPONGE = 2,
    NUVK_SDF_PRIMITIVE_TORUS         = 3,
    NUVK_SDF_PRIMITIVE_PLANE         = 4
} nuvk_sdf_primitives_t;

#endif