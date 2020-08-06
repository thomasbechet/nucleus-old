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

static void vertex_shader(vec3 pos, mat4 m, vec4 dest)
{
    /* multiply MVP */
    glm_vec4(pos, 1.0f, dest);
    glm_mat4_mulv(m, dest, dest);
}
/* Cohen Sutherland constants */
static bool clip_triangle(
    vec4 v[4],
    uint32_t indices[6],
    uint32_t *indice_count
)
{
    /* default triangle output */
    *indice_count = 3;
    indices[0] = 0;
    indices[1] = 1;
    indices[2] = 2;

    /* compute outsides */
    bool outside[3];
    for (uint32_t i = 0; i < 3; i++) {
        outside[i] = (v[i][3] <= 0) | (v[i][2] < -v[i][3]);
    }

    /* early test out */
    if ((outside[0] & outside[1] & outside[2]) != 0) return false;
    /* early test in  */
    if ((outside[0] | outside[1] | outside[2]) == 0) return true;

    /* clip required */
    for (uint32_t i = 0; i < 3; i++) {
        uint32_t id = i;
        uint32_t nid = (id + 1) % 3;
        if (outside[i] )
    }

    return false;
}
static void vertex_to_viewport(const vec3 v, vec4 vp, vec3 dest)
{
    /* (p + 1) / 2 */
    glm_vec3_adds((float*)v, 1.0f, dest);
    glm_vec3_scale(dest, 0.5f, dest);

    /* convert to viewport */
    dest[0] = dest[0] * vp[2] + vp[0];
    dest[1] = dest[1] * vp[3] + vp[1];
    dest[2] = v[2];
}

static float pixel_coverage(const vec2 a, const vec2 b, const vec2 c)
{
    return (c[0] - a[0]) * (b[1] - a[1]) - (c[1] - a[1]) * (b[0] - a[0]);
}

nu_result_t nusr_scene_initialize(void)
{
    /* camera */
    glm_vec3_zero(_data.camera.eye);
    glm_vec3_copy(_data.camera.center, (vec3){0, 0, -1});
    glm_vec3_copy(_data.camera.up, (vec3){0, 1, 0});
    _data.camera.fov = 90.0f;
    _data.camera.near = 0.1f;
    _data.camera.far = 1000.0f;

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
nu_result_t nusr_scene_render(nusr_framebuffer_t *color_buffer, nusr_framebuffer_t *depth_buffer)
{
    /* clear buffers */
    nusr_framebuffer_clear(color_buffer, 0x0);
    nusr_framebuffer_clear(depth_buffer, 0xFFFF7F7F);

    /* compute VP matrix from camera information */
    mat4 camera_projection, camera_view, vp;
    glm_lookat(_data.camera.eye, _data.camera.center, (vec3){0.0f, 1.0f, 0.0f}, camera_view);
    const float aspect = (double)color_buffer->width / (double)color_buffer->height;
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

        /* iterate over mesh triangles */
        for (uint32_t vi = 0; vi < mesh->vertex_count; vi += 3) {
            vec4 viewport = {0, 0, color_buffer->width, color_buffer->height};

            /* transform vertices */
            vec4 tv[4]; /* one vertices can be created for clipping step */
            vertex_shader(mesh->positions[vi + 0], mvp, tv[0]);
            vertex_shader(mesh->positions[vi + 1], mvp, tv[1]);
            vertex_shader(mesh->positions[vi + 2], mvp, tv[2]);

            /* clip vertices */
            uint32_t indices[6];
            uint32_t indice_count;
            if (!clip_triangle(tv, indices, &indice_count)) continue;

            for (uint32_t idx = 0; idx < indice_count; idx += 3) {
                vec3 v0, v1, v2;

                /* perspective divide (NDC) */
                glm_vec3_scale(tv[indices[idx + 0]], 1.0f / tv[indices[idx + 0]][3], v0);
                glm_vec3_scale(tv[indices[idx + 1]], 1.0f / tv[indices[idx + 1]][3], v1);
                glm_vec3_scale(tv[indices[idx + 2]], 1.0f / tv[indices[idx + 2]][3], v2);

                /* vertices to viewport */
                vertex_to_viewport(v0, viewport, v0);
                vertex_to_viewport(v1, viewport, v1);
                vertex_to_viewport(v2, viewport, v2);

                /* compute triangle viewport */
                vec4 tvp;
                float xmin = NU_MIN(v0[0], NU_MIN(v1[0], v2[0]));
                float xmax = NU_MAX(v0[0], NU_MAX(v1[0], v2[0]));
                float ymin = NU_MIN(v0[1], NU_MIN(v1[1], v2[1]));
                float ymax = NU_MAX(v0[1], NU_MAX(v1[1], v2[1]));
                tvp[0] = NU_MAX(0, xmin);
                tvp[1] = NU_MAX(0, ymin);
                tvp[2] = NU_MIN(viewport[2], xmax);
                tvp[3] = NU_MIN(viewport[3], ymax);

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

                            float depth = w0 * v0[2] + w1 * v1[2] + w2 * v2[2];
                            if (depth > 0.1f && depth < depth_buffer->pixels[j * depth_buffer->width + i].as_float) {
                                float r = w0 * c0[0] + w1 * c1[0] + w2 * c2[0];
                                float g = w0 * c0[1] + w1 * c1[1] + w2 * c2[1];
                                float b = w0 * c0[2] + w1 * c1[2] + w2 * c2[2];
                                depth_buffer->pixels[j * depth_buffer->width + i].as_float = depth;
                                nusr_framebuffer_set_rgb(color_buffer, i, j, r, g, b);
                            }
                        }
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