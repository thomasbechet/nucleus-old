#include "scene.h"

#include "../asset/mesh.h"
#include "../asset/texture.h"

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
    uint32_t texture;
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

static bool clip_triangle_and_perspective_devide(
    vec4 v[4],
    vec2 uv[4],
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

    /* perspective divide (NDC) */
    for (uint32_t i = 0; i < 3; i++) {
        glm_vec3_scale(v[i], 1.0f / v[i][3], v[i]);
    }

    /* early test out */
    if ((outside[0] & outside[1] & outside[2]) != 0) return false;
    /* early test in  */
    if ((outside[0] | outside[1] | outside[2]) == 0) return true;

    return false;

    /* clip required */
    for (uint32_t i = 0; i < 3; i++) {
        float *vec, *vec_prev, *vec_next;
        vec = v[i];
        vec_next = v[(i + 1) % 3];
        vec_prev = v[(i + 2) % 3];
        bool out, out_prev, out_next;
        out = outside[i];
        out_next = v[(i + 1) % 3];
        out_prev = v[(i + 2) % 3];
        
        const float zn = 0.1f;

        if (out) {
            if (out_next) { /* 2 out case 1 */
                float x0 = vec_prev[0];
                float y0 = vec_prev[1];
                float z0 = vec_prev[2];
                
                float x1 = vec[0];
                float y1 = vec[1];
                float z1 = vec[2];

                float s = (z0 - zn) / (z0 - z1);
                float ix = x0 + s * (x1 - x0);
                float iy = y0 + s * (y1 - y0);

                vec[0] = ix;
                vec[1] = iy;
                vec[2] = zn;
            } else if (out_prev) { /* 2 out case 2 */
                float x0 = vec_next[0];
                float y0 = vec_next[1];
                float z0 = vec_next[2];
                
                float x1 = vec[0];
                float y1 = vec[1];
                float z1 = vec[2];

                float s = (z0 - zn) / (z0 - z1);
                float ix = x0 + s * (x1 - x0);
                float iy = y0 + s * (y1 - y0);

                vec[0] = ix;
                vec[1] = iy;
                vec[2] = zn;
            } else { /* 1 out */
                return false;
                float x0, y0, z0;
                float s, ix, iy;
                float x1 = vec[0];
                float y1 = vec[1];
                float z1 = vec[2];
                /* prev lerp */
                x0 = vec_prev[0];
                y0 = vec_prev[1];
                z0 = vec_prev[2];
                s = (z0 - zn) / (z0 - z1);
                ix = x0 + s * (x1 - x0);
                iy = y0 + s * (y1 - y0);
                v[i][0] = ix;
                v[i][1] = iy;
                v[i][2] = zn;
                /* next lerp */
                x0 = vec_next[0];
                y0 = vec_next[1];
                z0 = vec_next[2];
                s = (z0 - zn) / (z0 - z1);
                ix = x0 + s * (x1 - x0);
                iy = y0 + s * (y1 - y0);
                v[3][0] = ix;
                v[3][1] = iy;
                v[3][2] = zn;
                uv[3][0] = 0;
                uv[3][1] = 0;
                *indice_count = 6;
                indices[3] = 3;
                indices[4] = (i + 2) % 3; 
                indices[5] = i;

                return true;
            }
        }

        // zn=0.4; //near plane distance
        // s=(z0-zn)/(z0-z1);
        // ix=x0+s*(x1-x0);
        // iy=y0+s*(y1-y0);
        // w = 1 / z

        // do uv clipping in linear space
    }

    return true;
}
static void vertex_to_viewport(const vec3 v, vec4 vp, vec3 dest)
{
    /* (p + 1) / 2 */
    glm_vec2_adds((float*)v, 1.0f, dest);
    glm_vec2_scale(dest, 0.5f, dest);

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

        /* access texture */
        nusr_texture_t *texture;
        nusr_texture_get(_data.staticmeshes[i].texture, &texture);

        /* iterate over mesh triangles */
        for (uint32_t vi = 0; vi < mesh->vertex_count; vi += 3) {
            vec4 viewport = {0, 0, color_buffer->width, color_buffer->height};
            vec4 tv[4]; /* one vertice can be added for the clipping step */
            vec2 uv[4]; /* one uv can be added for the clipping step */

            /* transform vertices */
            vertex_shader(mesh->positions[vi + 0], mvp, tv[0]);
            vertex_shader(mesh->positions[vi + 1], mvp, tv[1]);
            vertex_shader(mesh->positions[vi + 2], mvp, tv[2]);

            /* copy uvs */
            glm_vec2_copy(mesh->uvs[vi + 0], uv[0]);
            glm_vec2_copy(mesh->uvs[vi + 1], uv[1]);
            glm_vec2_copy(mesh->uvs[vi + 2], uv[2]);

            /* clip vertices */
            uint32_t indices[6];
            uint32_t indice_count;
            if (!clip_triangle_and_perspective_devide(tv, uv, indices, &indice_count)) continue;

            for (uint32_t idx = 0; idx < indice_count; idx += 3) {
                vec3 v0, v1, v2;
                vec2 uv0, uv1, uv2;

                /* vertices to viewport */
                vertex_to_viewport(tv[indices[idx + 0]], viewport, v0);
                vertex_to_viewport(tv[indices[idx + 1]], viewport, v1);
                vertex_to_viewport(tv[indices[idx + 2]], viewport, v2);

                /* perspective correct uv */
                // glm_vec2_scale(uv[indices[idx + 0]], v0[2], uv0);
                // glm_vec2_scale(uv[indices[idx + 1]], v1[2], uv1);
                // glm_vec2_scale(uv[indices[idx + 2]], v2[2], uv2);
                glm_vec2_copy(uv[indices[idx + 0]], uv0);
                glm_vec2_copy(uv[indices[idx + 1]], uv1);
                glm_vec2_copy(uv[indices[idx + 2]], uv2);

                /* correct z */
                v0[2] = 1.0f / v0[2];
                v1[2] = 1.0f / v1[2];
                v2[2] = 1.0f / v2[2];

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
                
                /* backface culling */
                float area = pixel_coverage(v0, v1, v2);
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

                            float depth = 1.0f / (w0 * v0[2] + w1 * v1[2] + w2 * v2[2]);
                            if (depth > 0.1f && depth < depth_buffer->pixels[j * depth_buffer->width + i].as_float) {
                                depth_buffer->pixels[j * depth_buffer->width + i].as_float = depth;
                                
                                float px = (w0 * uv0[0] + w1 * uv1[0] + w2 * uv2[0]) * texture->width;
                                float py = (w0 * uv0[1] + w1 * uv1[1] + w2 * uv2[1]) * texture->height;
                                //px *= depth;
                                //py *= depth;

                                uint32_t uvx = NU_MAX(0, NU_MIN(texture->width, (uint32_t)px));
                                uint32_t uvy = NU_MAX(0, NU_MIN(texture->height, (uint32_t)py));
                                
                                // float r = w0 * c0[0] + w1 * c1[0] + w2 * c2[0];
                                // float g = w0 * c0[1] + w1 * c1[1] + w2 * c2[1];
                                // float b = w0 * c0[2] + w1 * c1[2] + w2 * c2[2];
                                // r *= depth;
                                // g *= depth;
                                // b *= depth;
                                
                                // nusr_framebuffer_set_rgb(color_buffer, i, j, r, g, b);

                                uint32_t color = texture->data[uvy * texture->width + uvx];
                                color_buffer->pixels[j * color_buffer->width + i].as_uint = color;
                                //color_buffer->pixels[j * color_buffer->width + i].as_uint = 0x00FFFFFF;
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
    _data.staticmeshes[found_id].texture = info->texture;
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