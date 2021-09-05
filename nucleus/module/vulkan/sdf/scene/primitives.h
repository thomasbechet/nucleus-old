#ifndef NUVK_SDF_PRIMITIVES_H
#define NUVK_SDF_PRIMITIVES_H

#include <nucleus/module/vulkan/renderer.h>

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

typedef struct {
    nuvk_sdf_instance_type_t sphere;
    nuvk_sdf_instance_type_t cube;
    nuvk_sdf_instance_type_t menger_sponge;
    nuvk_sdf_instance_type_t torus;
    nuvk_sdf_instance_type_t plane;
} nuvk_sdf_primitives_t;

nu_result_t nuvk_sdf_primitives_register(nuvk_sdf_primitives_t *primitives);

#endif