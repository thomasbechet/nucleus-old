#include <nucleus/module/vulkan/sdf/scene/primitives.h>

/* sphere */

typedef struct { /* must match 16 bytes alignment (mat4 + data) */
    float radius;
    float _pad0[3];
} nuvk_sdf_sphere_data_t;

static const char *sphere_glsl_data_code =
    "float radius;";

static const char *sphere_glsl_sdf_code = 
    "return length(p) - data.radius;";

static float sphere_c_sdf_code(nu_vec3f_t p, const void *d)
{
    nuvk_sdf_sphere_data_t *data = (nuvk_sdf_sphere_data_t*)d;
    return nu_vec3f_length(p) - data->radius;
}

static void sphere_c_aabb_code(const void *d, nu_aabb_t *aabb)
{
    nuvk_sdf_sphere_data_t *data = (nuvk_sdf_sphere_data_t*)d;
    aabb->min[0] = aabb->min[1] = aabb->min[2] = -data->radius / 2.0f;
    aabb->max[0] = aabb->max[1] = aabb->max[2] = data->radius / 2.0f;
}

/* rectangle */
/* TODO */

nu_result_t nuvk_sdf_primitives_initialize(nuvk_sdf_primitives_t *primitives)
{
    primitives->sphere.data_size          = sizeof(nuvk_sdf_sphere_data_t);
    primitives->sphere.glsl_data_code     = sphere_glsl_data_code;
    primitives->sphere.glsl_sdf_code      = sphere_glsl_sdf_code;
    primitives->sphere.c_sdf_code         = sphere_c_sdf_code;
    primitives->sphere.c_aabb_code        = sphere_c_aabb_code;
    primitives->sphere.max_instance_count = 128;

    return NU_SUCCESS;
}