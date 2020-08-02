#include "scene.h"

#include "../asset/mesh.h"

#define MAX_STATICMESH_COUNT 512

typedef struct {
    vec3 eye;
    vec3 center;
    vec3 up;
    float fov;
    float near;
    float far;
} nusr_camera_t;

typedef struct {
    uint32_t mesh;
    mat4 transform;
    bool active;
} nusr_staticmesh_t;

typedef struct {
    nusr_camera_t camera;
    uint32_t staticmesh_count;
    nusr_staticmesh_t *staticmeshes;
} nusr_scene_data_t;

static nusr_scene_data_t _data;

static float pixel_coverage(const vec2 a, const vec2 b, const vec2 c)
{
    return (c[0] - a[0]) * (b[1] - a[1]) - (c[1] - a[1]) * (b[0] - a[0]);
}
static bool project_vertex(vec3 pos, mat4 m, vec4 vp, vec3 dest)
{
    CGLM_ALIGN(16) vec4 pos4, vone = GLM_VEC4_ONE_INIT;

    /* multiply MVP */
    glm_vec4(pos, 1.0f, pos4);
    glm_mat4_mulv(m, pos4, pos4);

    /* divide w */
    glm_vec4_scale(pos4, 1.0f / pos4[3], pos4);
    
    /* (p + 1) / 2 */
    glm_vec4_add(pos4, vone, pos4);
    glm_vec4_scale(pos4, 0.5f, pos4);

    /* convert to viewport */
    dest[0] = pos4[0] * vp[2] + vp[0];
    dest[1] = pos4[1] * vp[3] + vp[1];
    dest[2] = pos4[2];

    return true;
}
static void _vertex_transform(vec3 pos, mat4 m, vec4 dest)
{
    /* multiply MVP */
    glm_vec4(pos, 1.0f, dest);
    glm_mat4_mulv(m, dest, dest);
}
static void _vertex_raster(const vec4 vp, vec4 v, vec3 dest)
{
    /* (p + 1) / 2 */
    glm_vec4_adds(v, 1.0f, v);
    glm_vec4_scale(v, 0.5f, v);

    /* convert to viewport */
    dest[0] = v[0] * vp[2] + vp[0];
    dest[1] = v[1] * vp[3] + vp[1];
    dest[2] = v[2];
}
static bool clip_2D(const vec2 v0, const vec2 v1, const vec2 v2, const vec4 vp, vec4 tvp)
{
    float xmin = NU_MIN(v0[0], NU_MIN(v1[0], v2[0]));
    float xmax = NU_MAX(v0[0], NU_MAX(v1[0], v2[0]));
    float ymin = NU_MIN(v0[1], NU_MIN(v1[1], v2[1]));
    float ymax = NU_MAX(v0[1], NU_MAX(v1[1], v2[1]));

    if (xmin > vp[2] || xmax < 0 || ymin > vp[3] || ymax < 0) return false;

    tvp[0] = NU_MAX(0, xmin);
    tvp[1] = NU_MAX(0, ymin);
    tvp[2] = NU_MIN(vp[2], xmax);
    tvp[3] = NU_MIN(vp[3], ymax);

    return true;
}
static bool project_and_clip(
    const vec3 v0, const vec3 v1, const vec3 v2,
    vec3 out_v0, vec3 out_v1, vec3 out_v2,
    const mat4 mvp, const vec4 vp, vec4 tvp
)
{
    vec4 tv0, tv1, tv2;
    _vertex_transform((float*)v0, (vec4*)mvp, tv0);
    _vertex_transform((float*)v1, (vec4*)mvp, tv1);
    _vertex_transform((float*)v2, (vec4*)mvp, tv2);

    /* clip depth */
    if (tv0[3] <= 0 && tv1[3] <= 0 && tv2[3] <= 0) return false;

    bool clip = true;
    clip &= ((tv0[0] < -tv0[3] || tv0[0] > tv0[3]) || (tv0[1] < -tv0[3] || tv0[1] > tv0[3]) || (tv0[1] < -tv0[3] || tv0[1] > tv0[3]));
    clip &= ((tv1[0] < -tv1[3] || tv1[0] > tv1[3]) || (tv1[1] < -tv1[3] || tv1[1] > tv1[3]) || (tv1[1] < -tv1[3] || tv1[1] > tv1[3]));
    clip &= ((tv2[0] < -tv2[3] || tv2[0] > tv2[3]) || (tv2[1] < -tv2[3] || tv2[1] > tv2[3]) || (tv2[1] < -tv2[3] || tv2[1] > tv2[3]));
    if (clip) return false;

    /* divide w (NDC space) */
    glm_vec4_scale(tv0, 1.0f / tv0[3], tv0);
    glm_vec4_scale(tv1, 1.0f / tv1[3], tv1);
    glm_vec4_scale(tv2, 1.0f / tv2[3], tv2);

    /* project to viewport */
    _vertex_raster(vp, tv0, out_v0);
    _vertex_raster(vp, tv1, out_v1);
    _vertex_raster(vp, tv2, out_v2);

    /* compute triangle viewport */
    float xmin = NU_MIN(out_v0[0], NU_MIN(out_v1[0], out_v2[0]));
    float xmax = NU_MAX(out_v0[0], NU_MAX(out_v1[0], out_v2[0]));
    float ymin = NU_MIN(out_v0[1], NU_MIN(out_v1[1], out_v2[1]));
    float ymax = NU_MAX(out_v0[1], NU_MAX(out_v1[1], out_v2[1]));
    tvp[0] = NU_MAX(0, xmin);
    tvp[1] = NU_MAX(0, ymin);
    tvp[2] = NU_MIN(vp[2], xmax);
    tvp[3] = NU_MIN(vp[3], ymax);

    return true;
}

nu_result_t nusr_scene_initialize(void)
{
    /* camera */
    glm_vec3_zero(_data.camera.eye);
    glm_vec3_copy(_data.camera.center, (vec3){0, 0, -1});
    glm_vec3_copy(_data.camera.up, (vec3){0, 1, 0});
    _data.camera.fov = 90.0f;
    _data.camera.near = 0.1f;
    _data.camera.far = 100.0f;

    /* staticmesh */
    _data.staticmesh_count = 0;
    _data.staticmeshes = (nusr_staticmesh_t*)nu_malloc(sizeof(nusr_staticmesh_t) * MAX_STATICMESH_COUNT);
    for (uint32_t i = 0; i < MAX_STATICMESH_COUNT; i++) {
        _data.staticmeshes[i].active = false;
    }

    return NU_SUCCESS;
}
nu_result_t nusr_scene_terminate(void)
{
    nu_free(_data.staticmeshes);

    return NU_SUCCESS;
}
nu_result_t nusr_scene_render(nusr_framebuffer_t *framebuffer)
{
    /* compute VP matrix from camera information */
    mat4 camera_projection, camera_view, vp;
    glm_lookat(_data.camera.eye, _data.camera.center, (vec3){0.0f, 1.0f, 0.0f}, camera_view);
    const float aspect = (double)framebuffer->width / (double)framebuffer->height;
    glm_perspective(_data.camera.fov, aspect, _data.camera.near, _data.camera.far, camera_projection);
    glm_mat4_mul(camera_projection, camera_view, vp);

    /* iterate over staticmeshes */
    for (uint32_t i = 0; i < _data.staticmesh_count; i++) {
        if (!_data.staticmeshes[i].active) continue;

        /* compute mvp matrix */
        mat4 mvp;
        glm_mat4_mul(vp, _data.staticmeshes[i].transform, mvp);

        /* access mesh */
        nusr_mesh_t *mesh;
        nusr_mesh_get(_data.staticmeshes[i].mesh, &mesh);

        /* iterate over triangles */
        for (uint32_t vi = 0; vi < mesh->vertex_count; vi += 3) {
            vec4 viewport = {0, 0, framebuffer->width, framebuffer->height};
            
            vec4 tvp;
            vec3 v0, v1, v2;
            if (!project_and_clip(
                mesh->positions[vi + 0],
                mesh->positions[vi + 1],
                mesh->positions[vi + 2],
                v0, v1, v2,
                mvp, viewport, tvp
            )) continue;
            
            // clip = project_vertex(mesh->positions[vi + 0], mvp, viewport, v0);
            // clip = project_vertex(mesh->positions[vi + 1], mvp, viewport, v1);
            // clip = project_vertex(mesh->positions[vi + 2], mvp, viewport, v2);

            // /* compute raster rectangle */
            // if (!clip_2D(v0, v1, v2, viewport, tvp)) continue;

            /* compute edges */
            vec2 edge0, edge1, edge2;
            glm_vec2_sub(v2, v1, edge0);
            glm_vec2_sub(v0, v2, edge1);
            glm_vec2_sub(v1, v0, edge2);

            /* top left rule */
            bool t0 = (edge0[0] != 0) ? (edge0[0] > 0) : (edge0[1] > 0);
            bool t1 = (edge1[0] != 0) ? (edge1[0] > 0) : (edge1[1] > 0);
            bool t2 = (edge2[0] != 0) ? (edge2[0] > 0) : (edge2[1] > 0);

            /* colors */
            vec3 c0 = {1, 0, 0};
            vec3 c1 = {0, 1, 0};
            vec3 c2 = {0, 0, 1};

            float area = pixel_coverage(v0, v1, v2);
            
            /* backface culling */
            if (area < 0) continue;

            for (uint32_t j = tvp[1]; j < tvp[3]; j++) {
                for (uint32_t i = tvp[0]; i < tvp[2]; i++) {
                    vec2 sample = {i + 0.5, j + 0.5};

                    float w0 = pixel_coverage(v1, v2, sample);
                    float w1 = pixel_coverage(v2, v0, sample);
                    float w2 = pixel_coverage(v0, v1, sample);
                    
                    /* check sample with top left rule */
                    bool included = true;
                    included &= (w0 == 0) ? t0 : (w0 > 0);
                    included &= (w1 == 0) ? t1 : (w1 > 0);
                    included &= (w2 == 0) ? t2 : (w2 > 0);

                    if (included) {
                        const float area_inv = 1.0f / area;
                        w0 *= area_inv;
                        w1 *= area_inv;
                        w2 = 1.0f - w0 - w1;

                        float r = w0 * c0[0] + w1 * c1[0] + w2 * c2[0];
                        float g = w0 * c0[1] + w1 * c1[1] + w2 * c2[1];
                        float b = w0 * c0[2] + w1 * c1[2] + w2 * c2[2];

                        nusr_framebuffer_set_rgb(framebuffer, i, j, r, g, b);
                    }
                }
            }
        }
    }

    return NU_SUCCESS;
}

nu_result_t nusr_scene_camera_set_fov(float fov)
{
    _data.camera.fov = fov;
    return NU_SUCCESS;
}
nu_result_t nusr_scene_camera_set_eye(const vec3 eye)
{
    glm_vec3_copy((float*)eye, _data.camera.eye);
    return NU_SUCCESS;
}
nu_result_t nusr_scene_camera_set_center(const vec3 center)
{
    glm_vec3_copy((float*)center, _data.camera.center);
    return NU_SUCCESS;
}
nu_result_t nusr_scene_camera_set_transform(const mat4 transform)
{
    // vec3 temp;
    // glm_vec3_copy(temp, (vec3){0, 0, -1});
    // glm_mat4_mulv3(transform, temp, 0, temp);
    nu_fatal("not implemented.\n");
    return NU_SUCCESS;
}

nu_result_t nusr_scene_staticmesh_create(uint32_t *id, const nusr_staticmesh_create_info_t *info)
{
    uint32_t found_id = MAX_STATICMESH_COUNT;
    for (uint32_t i = 0; i < _data.staticmesh_count; i++) {
        if (!_data.staticmeshes[i].active) {
            found_id = i;
            break;
        }
    }

    if (found_id == MAX_STATICMESH_COUNT) {
        if (_data.staticmesh_count >= MAX_STATICMESH_COUNT) return NU_FAILURE;
        found_id = _data.staticmesh_count;
        _data.staticmesh_count++;
    }

    _data.staticmeshes[found_id].active = true;
    _data.staticmeshes[found_id].mesh = info->mesh;
    glm_mat4_copy((vec4*)info->transform, _data.staticmeshes[found_id].transform);

    *id = found_id;

    return NU_SUCCESS;
}
nu_result_t nusr_scene_staticmesh_destroy(uint32_t id)
{
    if (!_data.staticmeshes[id].active) return NU_FAILURE;

    _data.staticmeshes[id].active = false;

    return NU_SUCCESS;
}
nu_result_t nusr_scene_staticmesh_set_transform(uint32_t id, const mat4 m)
{
    if (!_data.staticmeshes[id].active) return NU_FAILURE;

    glm_mat4_copy((vec4*)m, _data.staticmeshes[id].transform);

    return NU_SUCCESS;
}