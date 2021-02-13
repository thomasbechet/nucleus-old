#include <nucleus/module/softrast/scene/scene.hpp>

#include <array>

using namespace nu::softrast;

Scene::Scene()
{
    // Initialize camera
    m_camera.view = Matrix4f::identity();
    m_camera.fov  = nu_radian(80.0f);
    m_camera.near = 0.1f;
    m_camera.far  = 100.0f;
}

void Scene::setCameraFov(float fov)
{
    m_camera.fov = fov;
}
void Scene::setCameraView(const Vector3f &eye, const Vector3f &forward, const Vector3f &up)
{
    m_camera.view = Matrix4f::lookAt(eye, eye + forward, up);
}

nu_renderer_model_handle_t Scene::createModel(const nu_renderer_model_create_info_t &info)
{
    uint32_t id = m_nextId++;
    m_models.emplace(id, Model(info));
    nu_renderer_model_handle_t handle;
    NU_HANDLE_SET_ID(handle, id);
    return handle;
}
void Scene::destroyModel(nu_renderer_model_handle_t handle)
{
    uint32_t id; NU_HANDLE_GET_ID(handle, id);
    m_models.erase(id);
}
void Scene::setModelTransform(nu_renderer_model_handle_t handle, const Matrix4f &transform)
{
    uint32_t id; NU_HANDLE_GET_ID(handle, id);
    m_models.at(id).transform = transform;
}

static void vertexShader(
    const Vertex &input, 
    const nu_mat4f_t transform, 
    nu_vec4f_t position,
    nu_vec2f_t uv
)
{
    // Multiply MVP
    nu_vec4f_from_v3(input.position, 1.0f, position);
    nu_mat4f_mulv(transform, position, position);
    
    // Copy uv
    nu_vec2f_copy(input.uv, uv);
}
static void clipEdgeNear(
    nu_vec4f_t v0, nu_vec4f_t v1, nu_vec4f_t vclip,
    nu_vec2f_t uv0, nu_vec2f_t uv1, nu_vec2f_t uvclip
)
{
    const nu_vec4f_t near_plane = {0, 0, 1, 1};
    float d0 = nu_vec4f_dot(v0, near_plane);
    float d1 = nu_vec4f_dot(v1, near_plane);
    float s = d0 / (d0 - d1);
    nu_vec4f_lerp(v0, v1, s, vclip);
    nu_vec2f_lerp(uv0, uv1, s, uvclip);
}
static bool clipTriangle(
    nu_vec4f_t vertices[4],
    nu_vec2f_t uvs[4],
    std::array<uint32_t, 6> &indices,
    uint32_t &indiceCount
)
{
    // Default triangle output
    indiceCount = 3;
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
            if (out_next) { // 2 out case 1
                clipEdgeNear(vec, vec_prev, vec, uv, uv_prev, uv);
            } else if (out_prev) { // 2 out case 2
                clipEdgeNear(vec, vec_next, vec, uv, uv_next, uv);
            } else { // 1 out
                // Produce new vertex
                clipEdgeNear(vec, vec_next, vertices[3], uv, uv_next, uvs[3]);
                indiceCount = 6;
                indices[3] = i;
                indices[4] = 3; // New vertex
                indices[5] = (i + 1) % 3;

                // Clip existing vertex
                clipEdgeNear(vec, vec_prev, vec, uv, uv_prev, uv);

                return true;
            }
        }
    }

    return true;
}
static void positionToViewport(const nu_vec4f_t vp, nu_vec2f_t v)
{
    /* (p + 1) / 2 */
    nu_vec2f_adds(v, 1.0f, v);
    nu_vec2f_muls(v, 0.5f, v);
    
    /* convert to viewport */
    nu_vec2f_mul(v, vp + 2, v);
    nu_vec2f_add(v, vp + 0, v);
}
static float pixelCoverage(const nu_vec2f_t a, const nu_vec2f_t b, const nu_vec2f_t c)
{
    return (c[0] - a[0]) * (b[1] - a[1]) - (c[1] - a[1]) * (b[0] - a[0]);
}
void Scene::draw(ColorFramebuffer &colorBuffer, Framebuffer<float> &depthBuffer, const AssetManager &assetManager) const
{
    // Get buffer size
    uint32_t width, height;
    width = colorBuffer.getSize().x;
    height = colorBuffer.getSize().y;
    const nu_vec4f_t viewport = {0, 0, (float)width, (float)height};

    // Compute VP matrix
    nu_mat4f_t camera_projection, vp;
    const float aspect = (float)width / (float)height;

    nu_perspective(m_camera.fov, aspect, m_camera.near, m_camera.far, camera_projection);
    nu_mat4f_mul(camera_projection, m_camera.view, vp);

    // Iterate over models
    for (auto &any : m_models) {
        const Model &model = any.second;

        // Compute mvp matrix
        nu_mat4f_t mvp;
        nu_mat4f_mul(vp, model.transform, mvp);

        // Access mesh
        const Mesh &mesh = assetManager.getMesh(model.mesh);

        // Iterate over submeshes
        for (uint32_t submeshIndex = 0; submeshIndex < mesh.submeshes.size(); submeshIndex++) {
            const auto &vertices = mesh.submeshes[submeshIndex];
            const auto &material = assetManager.getMaterial(model.materials[submeshIndex]);
            const auto &texture  = assetManager.getTexture(material.texture);

            // Iterate over triangles
            for (uint32_t vi = 0; vi < vertices.size(); vi += 3) {
                nu_vec4f_t tv[4];
                nu_vec2f_t uv[4];

                // Apply vertex shader
                vertexShader(vertices[vi + 1], mvp, tv[0], uv[0]);
                vertexShader(vertices[vi + 0], mvp, tv[1], uv[1]);
                vertexShader(vertices[vi + 2], mvp, tv[2], uv[2]);

                // Clip vertices
                std::array<uint32_t, 6> indices;
                uint32_t indiceCount;
                if (!clipTriangle(tv, uv, indices, indiceCount)) continue;

                // Perspective divide (NDC)
                uint32_t totalVertex = (indiceCount > 3) ? 4 : 3;
                for (uint32_t i = 0; i < totalVertex; i++) {
                    nu_vec3f_muls(tv[i], 1.0f / tv[i][3], tv[i]);
                }

                // Iterate over clipped triangles
                for (uint32_t idx = 0; idx < indiceCount; idx += 3) {
                    nu_vec4f_t v0, v1, v2;
                    nu_vec2f_t uv0, uv1, uv2;

                    // Vertices to viewport
                    nu_vec4f_copy(tv[indices[idx + 0]], v0);
                    nu_vec4f_copy(tv[indices[idx + 1]], v1);
                    nu_vec4f_copy(tv[indices[idx + 2]], v2);
                    positionToViewport(viewport, v0);
                    positionToViewport(viewport, v1);
                    positionToViewport(viewport, v2);

                    // Copy uvs
                    nu_vec2f_copy(uv[indices[idx + 0]], uv0);
                    nu_vec2f_copy(uv[indices[idx + 1]], uv1);
                    nu_vec2f_copy(uv[indices[idx + 2]], uv2);

                    // Backface culling
                    float area = pixelCoverage(v0, v1, v2);
                    if (area <= 0) continue;

                    // Compute triangle viewport
                    nu_vec4i_t tvp;
                    int32_t xmin = (int32_t)NU_MIN(v0[0], NU_MIN(v1[0], v2[0]));
                    int32_t xmax = (int32_t)NU_MAX(v0[0], NU_MAX(v1[0], v2[0]));
                    int32_t ymin = (int32_t)NU_MIN(v0[1], NU_MIN(v1[1], v2[1]));
                    int32_t ymax = (int32_t)NU_MAX(v0[1], NU_MAX(v1[1], v2[1]));
                    tvp[0] = NU_MAX(0, xmin);
                    tvp[1] = NU_MAX(0, ymin);
                    tvp[2] = NU_MIN((int32_t)viewport[2], xmax);
                    tvp[3] = NU_MIN((int32_t)viewport[3], ymax);

                    // Compute edges
                    nu_vec2f_t edge0, edge1, edge2;
                    nu_vec2f_sub(v2, v1, edge0);
                    nu_vec2f_sub(v0, v2, edge1);
                    nu_vec2f_sub(v1, v0, edge2);
                
                    // Top left rule
                    bool t0 = (edge0[0] != 0) ? (edge0[0] > 0) : (edge0[1] > 0);
                    bool t1 = (edge1[0] != 0) ? (edge1[0] > 0) : (edge1[1] > 0);
                    bool t2 = (edge2[0] != 0) ? (edge2[0] > 0) : (edge2[1] > 0);

                    float inv_vw0 = 1.0f / v0[3];
                    float inv_vw1 = 1.0f / v1[3];
                    float inv_vw2 = 1.0f / v2[3];

                    // Iterate over pixels
                    for (int32_t j = tvp[1]; j < tvp[3]; j++) {
                        for (int32_t i = tvp[0]; i < tvp[2]; i++) {
                            nu_vec2f_t sample = {i + 0.5f, j + 0.5f};

                            // Compute weights
                            float w0 = pixelCoverage(v1, v2, sample);
                            float w1 = pixelCoverage(v2, v0, sample);
                            float w2 = pixelCoverage(v0, v1, sample);

                            // Check sample with the top left rule
                            bool included = true;
                            included &= (w0 == 0.0f) ? t0 : (w0 > 0.0f);
                            included &= (w1 == 0.0f) ? t1 : (w1 > 0.0f);
                            included &= (w2 == 0.0f) ? t2 : (w2 > 0.0f);

                            // Compute pixel
                            if (included) {
                                const float area_inv = 1.0f / area;
                                w0 *= area_inv;
                                w1 *= area_inv;
                                w2 = 1.0f - w0 - w1;

                                float a = w0 * inv_vw0;
                                float b = w1 * inv_vw1;
                                float c = w2 * inv_vw2;
                                float inv_sum_abc = 1.0f / (a + b + c);
                                a *= inv_sum_abc;
                                b *= inv_sum_abc;
                                c *= inv_sum_abc;

                                /* depth test */
                                float depth = (a * v0[3] + b * v1[3] + c * v2[3]);
                                if (depth < depthBuffer.get(i, j)) {
                                    depthBuffer.set(i, j, depth);
                                    
                                    /* correct linear interpolation */

                                    /*     a * f_a / w_a   +   b * f_b / w_b   +  c * f_c / w_c  *
                                    * f=-----------------------------------------------------   *
                                    *        a / w_a      +      b / w_b      +     c / w_c     */

                                    uint32_t textureWidth = texture.getSize().x;
                                    uint32_t textureHeight = texture.getSize().y;
                                    
                                    float px = (a * uv0[0] + b * uv1[0] + c * uv2[0]) * textureWidth;
                                    float py = (a * uv0[1] + b * uv1[1] + c * uv2[1]) * textureHeight;

                                    //float px = (w0 * uv0[0] + w1 * uv1[0] + w2 * uv2[0]) * textureWidth;
                                    //float py = (w0 * uv0[1] + w1 * uv1[1] + w2 * uv2[1]) * textureHeight;

                                    uint32_t uvx = NU_MAX(0, NU_MIN(textureWidth - 1, (uint32_t)px));
                                    uint32_t uvy = NU_MAX(0, NU_MIN(textureHeight - 1, textureHeight - 1 - (uint32_t)py));

                                    uint32_t color = texture.get(uvx, uvy);
                                    colorBuffer.set(i, height - j - 1, color);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}