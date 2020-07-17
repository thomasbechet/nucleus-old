#include "scene.h"

#include "../asset/mesh.h"

#define MAX_STATICMESH_COUNT 64

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

static float edge_function(const vec2 a, const vec2 b, const vec2 c)
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
            
            vec3 v0, v1, v2;
            glm_project(mesh->positions[vi + 0], mvp, viewport, v0);
            glm_project(mesh->positions[vi + 1], mvp, viewport, v1);
            glm_project(mesh->positions[vi + 2], mvp, viewport, v2);

            /* compute raster rectangle */
            uint32_t wmin, wmax, hmin, hmax;
            wmin = NU_MIN(v0[0], NU_MIN(v1[0], v2[0]));
            wmax = NU_MAX(v0[0], NU_MAX(v1[0], v2[0]));
            hmin = NU_MIN(v0[1], NU_MIN(v1[1], v2[1]));
            hmax = NU_MAX(v0[1], NU_MAX(v1[1], v2[1]));

            vec3 c0 = {1, 0, 0};
            vec3 c1 = {0, 1, 0};
            vec3 c2 = {0, 0, 1};

            float area = edge_function(v0, v1, v2);

            for (uint32_t j = hmin; j < hmax; j++) {
                for (uint32_t i = wmin; i < wmax; i++) {
                    vec2 p = {i + 0.5, j + 0.5};
                    float w0 = edge_function(v1, v2, p);
                    float w1 = edge_function(v2, v0, p);
                    float w2 = edge_function(v0, v1, p);
                    if (w0 >= 0 && w1 >= 0 && w2 >= 0) {
                        w0 /= area;
                        w1 /= area;
                        w2 /= area;

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