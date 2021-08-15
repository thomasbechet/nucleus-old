#include <nucleus/module/vulkan/sdf/entity/primitives.h>

/* sphere */

typedef struct {
    float radius;
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

nu_result_t nuvk_sdf_primitives_register(nuvk_sdf_primitives_t *primitives)
{
    nuvk_sdf_instance_type_info_t sphere_info;
    memset(&sphere_info, 0, sizeof(nuvk_sdf_instance_type_info_t));
    sphere_info.data_size      = sizeof(nuvk_sdf_sphere_data_t);
    sphere_info.glsl_data_code = sphere_glsl_data_code;
    sphere_info.glsl_sdf_code  = sphere_glsl_sdf_code;
    sphere_info.c_sdf_code     = sphere_c_sdf_code;
    sphere_info.c_aabb_code    = sphere_c_aabb_code;

    return NU_SUCCESS;
}