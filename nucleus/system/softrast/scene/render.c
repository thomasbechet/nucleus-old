#include "render.h"

#include "../asset/font.h"
#include "../asset/mesh.h"
#include "../asset/texture.h"

static void vertex_shader(nu_vec3_t pos, nu_mat4_t m, nu_vec4_t dest)
{
    /* multiply MVP */
    nu_vec4(pos, 1.0f, dest);
    nu_mat4_mulv(m, dest, dest);
}

static void clip_edge_near(
    nu_vec4_t v0, nu_vec4_t v1, nu_vec4_t vclip,
    nu_vec2_t uv0, nu_vec2_t uv1, nu_vec2_t uvclip
)
{
    const nu_vec4_t near_plane = {0, 0, 1, 1};
    float d0 = nu_vec4_dot(v0, near_plane);
    float d1 = nu_vec4_dot(v1, near_plane);
    float s = d0 / (d0 - d1);
    nu_vec4_lerp(v0, v1, s, vclip);
    nu_vec2_lerp(uv0, uv1, s, uvclip);
}
static bool clip_triangle(
    nu_vec4_t vertices[4],
    nu_vec2_t uvs[4],
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
        outside[i] = (vertices[i][3] <= 0) || (vertices[i][2] < -vertices[i][3]);
    }

    /* early test out */
    if ((outside[0] & outside[1] & outside[2]) != 0) return false;
    /* early test in  */
    if ((outside[0] | outside[1] | outside[2]) == 0) return true;

    /* clip vertices */
    for (uint32_t i = 0; i < 3; i++) {
        float *vec, *vec_prev, *vec_next;
        float *uv, *uv_prev, *uv_next;
        bool out, out_prev, out_next;
        vec = vertices[i];
        uv = uvs[i];
        out = outside[i];

        vec_next = vertices[(i + 1) % 3];
        uv_next = uvs[(i + 1) % 3];
        out_next = outside[(i + 1) % 3];
        
        vec_prev = vertices[(i + 2) % 3];
        uv_prev = uvs[(i + 2) % 3];
        out_prev = outside[(i + 2) % 3];

        if (out) {
            if (out_next) { /* 2 out case 1 */
                clip_edge_near(vec, vec_prev, vec, uv, uv_prev, uv);
            } else if (out_prev) { /* 2 out case 2 */
                clip_edge_near(vec, vec_next, vec, uv, uv_next, uv);
            } else { /* 1 out */
                /* produce new vertex */
                clip_edge_near(vec, vec_next, vertices[3], uv, uv_next, uvs[3]);
                *indice_count = 6;
                indices[3] = i;
                indices[4] = 3; /* new vertex */
                indices[5] = (i + 1) % 3;

                /* clip existing vertex */
                clip_edge_near(vec, vec_prev, vec, uv, uv_prev, uv);

                return true;
            }
        }
    }

    return true;
}
static void vertex_to_viewport(nu_vec2_t v, nu_vec4_t vp)
{
    /* (p + 1) / 2 */
    nu_vec2_adds(v, 1.0f, v);
    nu_vec2_muls(v, 0.5f, v);
    
    /* convert to viewport */
    nu_vec2_mul(v, vp + 2, v);
    nu_vec2_add(v, vp + 0, v);
}

static float pixel_coverage(const nu_vec2_t a, const nu_vec2_t b, const nu_vec2_t c)
{
    return (c[0] - a[0]) * (b[1] - a[1]) - (c[1] - a[1]) * (b[0] - a[0]);
}

nu_result_t nusr_scene_render_global(
    nusr_renderbuffer_t *renderbuffer,
    const nusr_camera_t *camera,
    const nusr_staticmesh_t *staticmeshes,
    uint32_t staticmesh_count
)
{
    /* clear buffers */
    nusr_framebuffer_clear(color_buffer, 0x0);
    nusr_framebuffer_clear(depth_buffer, 0xFFFF7F7F); /* max float value */

    /* compute VP matrix from camera information */
    nu_mat4_t camera_projection, camera_view, vp;
    nu_lookat(camera->eye, camera->center, (nu_vec3_t){0.0f, 1.0f, 0.0f}, camera_view);
    const float aspect = (double)color_buffer->width / (double)color_buffer->height;
    nu_perspective(camera->fov, aspect, camera->near, camera->far, camera_projection);
    nu_mat4_mul(camera_projection, camera_view, vp);

    /* iterate over staticmeshes */
    for (uint32_t i = 0; i < staticmesh_count; i++) {
        if (!staticmeshes[i].active) continue;

        /* compute mvp matrix */
        nu_mat4_t mvp;
        nu_mat4_mul(vp, staticmeshes[i].transform, mvp);

        /* access mesh */
        nusr_mesh_t *mesh;
        nusr_mesh_get(staticmeshes[i].mesh, &mesh);

        /* access texture */
        nusr_texture_t *texture;
        nusr_texture_get(staticmeshes[i].texture, &texture);

        /* iterate over mesh triangles */
        for (uint32_t vi = 0; vi < mesh->vertex_count; vi += 3) {
            nu_vec4_t viewport = {0, 0, color_buffer->width, color_buffer->height};
            nu_vec4_t tv[4]; /* one vertice can be added for the clipping step */
            nu_vec2_t uv[4]; /* one uv can be added for the clipping step */

            /* transform vertices */
            vertex_shader(mesh->positions[vi + 0], mvp, tv[0]);
            vertex_shader(mesh->positions[vi + 1], mvp, tv[1]);
            vertex_shader(mesh->positions[vi + 2], mvp, tv[2]);

            /* copy uv (should be done in vertex shader) */
            nu_vec2_copy(mesh->uvs[vi + 0], uv[0]);
            nu_vec2_copy(mesh->uvs[vi + 1], uv[1]);
            nu_vec2_copy(mesh->uvs[vi + 2], uv[2]);

            /* clip vertices */
            uint32_t indices[6];
            uint32_t indice_count;
            if (!clip_triangle(tv, uv, indices, &indice_count)) continue;

            /* perspective divide (NDC) */
            uint32_t total_vertex = (indice_count > 3) ? 4 : 3;
            for (uint32_t i = 0; i < total_vertex; i++) {
                nu_vec3_muls(tv[i], 1.0f / tv[i][3], tv[i]);
            }
            
            for (uint32_t idx = 0; idx < indice_count; idx += 3) {
                nu_vec4_t v0, v1, v2;
                nu_vec2_t uv0, uv1, uv2;

                /* vertices to viewport */
                nu_vec4_copy(tv[indices[idx + 0]], v0);
                nu_vec4_copy(tv[indices[idx + 1]], v1);
                nu_vec4_copy(tv[indices[idx + 2]], v2);
                vertex_to_viewport(v0, viewport);
                vertex_to_viewport(v1, viewport);
                vertex_to_viewport(v2, viewport);

                /* copy uvs */
                nu_vec2_copy(uv[indices[idx + 0]], uv0);
                nu_vec2_copy(uv[indices[idx + 1]], uv1);
                nu_vec2_copy(uv[indices[idx + 2]], uv2);

                /* backface culling */
                float area = pixel_coverage(v0, v1, v2);
                if (area < 0) continue;

                /* compute triangle viewport */
                nu_vec4_t tvp;
                float xmin = NU_MIN(v0[0], NU_MIN(v1[0], v2[0]));
                float xmax = NU_MAX(v0[0], NU_MAX(v1[0], v2[0]));
                float ymin = NU_MIN(v0[1], NU_MIN(v1[1], v2[1]));
                float ymax = NU_MAX(v0[1], NU_MAX(v1[1], v2[1]));
                tvp[0] = NU_MAX(0, xmin);
                tvp[1] = NU_MAX(0, ymin);
                tvp[2] = NU_MIN(viewport[2], xmax);
                tvp[3] = NU_MIN(viewport[3], ymax);

                /* compute edges */
                nu_vec2_t edge0, edge1, edge2;
                nu_vec2_sub(v2, v1, edge0);
                nu_vec2_sub(v0, v2, edge1);
                nu_vec2_sub(v1, v0, edge2);

                /* top left rule */
                bool t0 = (edge0[0] != 0) ? (edge0[0] > 0) : (edge0[1] > 0);
                bool t1 = (edge1[0] != 0) ? (edge1[0] > 0) : (edge1[1] > 0);
                bool t2 = (edge2[0] != 0) ? (edge2[0] > 0) : (edge2[1] > 0);

                float inv_vw0 = 1.0f / v0[3];
                float inv_vw1 = 1.0f / v1[3];
                float inv_vw2 = 1.0f / v2[3];

                for (uint32_t j = tvp[1]; j < tvp[3]; j++) {
                    for (uint32_t i = tvp[0]; i < tvp[2]; i++) {
                        nu_vec2_t sample = {i + 0.5, j + 0.5};

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

                            /* depth test */
                            float depth = (w0 * v0[3] + w1 * v1[3] + w2 * v2[3]);
                            if (depth < depth_buffer->pixels[j * depth_buffer->width + i].as_float) {
                                depth_buffer->pixels[j * depth_buffer->width + i].as_float = depth;
                                
                                /* correct linear interpolation */

                                /*     a * f_a / w_a   +   b * f_b / w_b   +  c * f_c / w_c  *
                                 * f=-----------------------------------------------------   *
                                 *        a / w_a      +      b / w_b      +     c / w_c     */
                                 
                                float a = w0 * inv_vw0;
                                float b = w1 * inv_vw1;
                                float c = w2 * inv_vw2;
                                float inv_sum_abc = 1.0f / (a + b + c);
                                a *= inv_sum_abc;
                                b *= inv_sum_abc;
                                c *= inv_sum_abc;

                                float px = (a * uv0[0] + b * uv1[0] + c * uv2[0]) * texture->width;
                                float py = (a * uv0[1] + b * uv1[1] + c * uv2[1]) * texture->height;

                                // float px = (w0 * uv0[0] + w1 * uv1[0] + w2 * uv2[0]) * texture->width;
                                // float py = (w0 * uv0[1] + w1 * uv1[1] + w2 * uv2[1]) * texture->height;

                                uint32_t uvx = NU_MAX(0, NU_MIN(texture->width, (uint32_t)px));
                                uint32_t uvy = NU_MAX(0, NU_MIN(texture->height, (uint32_t)py));

                                uint32_t color = texture->data[uvy * texture->width + uvx];
                                nusr_framebuffer_set_uint(color_buffer, i, color_buffer->height - j - 1, color);
                            }
                        }
                    }
                }
            }
        }
    }

    return NU_SUCCESS;
}