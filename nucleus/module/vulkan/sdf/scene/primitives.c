#include <nucleus/module/vulkan/sdf/scene/primitives.h>

#include <nucleus/module/vulkan/renderer.h>

/* sphere */

static const char *sphere_glsl_data_code =
    "float radius;";

static const char *sphere_glsl_sdf_code = 
    "return length(p) - 1.0;";

static float sphere_c_sdf_code(const nu_vec3f_t p, const void *d)
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

/* cube */

static const char *cube_glsl_data_code =
    "vec3 box;"
    "float radius;";

static const char *cube_glsl_sdf_code = 
    "vec3 q = abs(p) - data.box;\n"
    "return length(max(q, 0.0)) + min(max(q.x, max(q.y, q.z)), 0.0) - data.radius;";

static float cube_c_sdf_code(const nu_vec3f_t p, const void *d)
{
    nuvk_sdf_cube_data_t *data = (nuvk_sdf_cube_data_t*)d;
    nu_vec3f_t vec;
    nu_vec3f_copy(p, vec); 
    nu_vec3f_abs(vec); /* abs(p) */
    nu_vec3f_sub(vec, data->box, vec); /* - data.box */
    nu_vec3f_maxv(vec, NU_VEC3F_ZERO, vec); /* max(q, 0) */
    return nu_vec3f_length(vec) + NU_MIN(NU_MAX(vec[0], NU_MAX(vec[1], vec[2])), 0.0f) - data->radius;
}

static void cube_c_aabb_code(const void *d, nu_aabb_t *aabb)
{
    nu_vec3f_zero(aabb->min);
    nu_vec3f_zero(aabb->max);
}

/* menger sponge */

static const char *menger_sponge_glsl_data_code =
    "vec4 unused;";

static const char *menger_sponge_glsl_sdf_code = 
    "const float scale = 30.0f;\n"
    "vec3 q = abs(p / scale) - vec3(0, -0.5, 0) - vec3(100, 1, 100);\n"
    "float d = length(max(q, 0)) + min(max(q.x, max(q.y, q.z)), 0);\n"
    "float s = 2.67;\n"
    "for (int m = 0; m < 7; m++) {\n"
    "    vec3 a = mod(q * s, 2.0) - 1.0;\n"
    "    s *= 3.0;\n"
    "    vec3 r = abs(1.0 - 3.0 * abs(a));\n"
    "    float da = max(r.x, r.y);\n"
    "    float db = max(r.y, r.z);\n"
    "    float dc = max(r.z, r.x);\n"
    "    float c = (min(da, min(db, dc)) - 1.0) / s;\n"
    "    d = max(d, c);\n"
    "}\n"
    "return d * scale;\n";

static float menger_sponge_c_sdf_code(const nu_vec3f_t p, const void *d)
{
    return 0.0f;
}

static void menger_sponge_c_aabb_code(const void *d, nu_aabb_t *aabb)
{
    nu_vec3f_zero(aabb->min);
    nu_vec3f_zero(aabb->max);
}

/* torus */

static const char *torus_glsl_data_code =
    "vec2 dim;";

static const char *torus_glsl_sdf_code = 
    "vec2 q = vec2(length(p.xz) - data.dim.x, p.y);\n"
    "return length(q) - data.dim.y;";

static float torus_c_sdf_code(const nu_vec3f_t p, const void *d)
{
    return 0.0f;
}

static void torus_c_aabb_code(const void *d, nu_aabb_t *aabb)
{
    nu_vec3f_zero(aabb->min);
    nu_vec3f_zero(aabb->max);
}

/* plane */

static const char *plane_glsl_data_code =
    "vec3 normal;\n"
    "float height;";

static const char *plane_glsl_sdf_code = 
    "return dot(p, data.normal) - data.height;";

static float plane_c_sdf_code(const nu_vec3f_t p, const void *d)
{
    return 0.0f;
}

static void plane_c_aabb_code(const void *d, nu_aabb_t *aabb)
{
    nu_vec3f_zero(aabb->min);
    nu_vec3f_zero(aabb->max);
}

nu_result_t nuvk_sdf_primitives_register(nuvk_sdf_instance_type_t *types)
{
    nu_result_t result = NU_SUCCESS;
    nuvk_sdf_instance_type_info_t info;

    memset(&info, 0, sizeof(nuvk_sdf_instance_type_info_t));
    info.data_size          = sizeof(nuvk_sdf_sphere_data_t);
    info.glsl_data_code     = sphere_glsl_data_code;
    info.glsl_sdf_code      = sphere_glsl_sdf_code;
    info.c_sdf_code         = sphere_c_sdf_code;
    info.c_aabb_code        = sphere_c_aabb_code;
    info.max_instance_count = 128;

    result &= nuvk_sdf_instance_type_register(&info, &types[NUVK_SDF_INSTANCE_TYPE_SPHERE]);

    memset(&info, 0, sizeof(nuvk_sdf_instance_type_info_t));
    info.data_size          = sizeof(nuvk_sdf_cube_data_t);
    info.glsl_data_code     = cube_glsl_data_code;
    info.glsl_sdf_code      = cube_glsl_sdf_code;
    info.c_sdf_code         = cube_c_sdf_code;
    info.c_aabb_code        = cube_c_aabb_code;
    info.max_instance_count = 128;

    result &= nuvk_sdf_instance_type_register(&info, &types[NUVK_SDF_INSTANCE_TYPE_CUBE]);

    memset(&info, 0, sizeof(nuvk_sdf_instance_type_info_t));
    info.data_size          = sizeof(nuvk_sdf_menger_sponge_data_t);
    info.glsl_data_code     = menger_sponge_glsl_data_code;
    info.glsl_sdf_code      = menger_sponge_glsl_sdf_code;
    info.c_sdf_code         = menger_sponge_c_sdf_code;
    info.c_aabb_code        = menger_sponge_c_aabb_code;
    info.max_instance_count = 128;

    result &= nuvk_sdf_instance_type_register(&info, &types[NUVK_SDF_INSTANCE_TYPE_MENGER_SPONGE]);

    memset(&info, 0, sizeof(nuvk_sdf_instance_type_info_t));
    info.data_size          = sizeof(nuvk_sdf_torus_data_t);
    info.glsl_data_code     = torus_glsl_data_code;
    info.glsl_sdf_code      = torus_glsl_sdf_code;
    info.c_sdf_code         = torus_c_sdf_code;
    info.c_aabb_code        = torus_c_aabb_code;
    info.max_instance_count = 128;

    result &= nuvk_sdf_instance_type_register(&info, &types[NUVK_SDF_INSTANCE_TYPE_TORUS]);

    memset(&info, 0, sizeof(nuvk_sdf_instance_type_info_t));
    info.data_size          = sizeof(nuvk_sdf_plane_data_t);
    info.glsl_data_code     = plane_glsl_data_code;
    info.glsl_sdf_code      = plane_glsl_sdf_code;
    info.c_sdf_code         = plane_c_sdf_code;
    info.c_aabb_code        = plane_c_aabb_code;
    info.max_instance_count = 128;

    result &= nuvk_sdf_instance_type_register(&info, &types[NUVK_SDF_INSTANCE_TYPE_PLANE]);

    return NU_SUCCESS;
}