#include "raytracer.hpp"

#include "../scene/object/sphere.hpp"
#include "../scene/object/plane.hpp"

#include <nucleus/nucleus.h>

using namespace nurt;

Raytracer::Raytracer() : m_prng(0)
{
    nu_config_get_uint("raytracer", "width", 640, &m_width);
    nu_config_get_uint("raytracer", "height", 360, &m_height);
    nu_config_get_uint("raytracer", "sample_count", 32, &m_sampleCount);
    nu_config_get_uint("raytracer", "max_depth", 8, &m_maxDepth);
    nu_config_get_float("raytracer", "bias", 0.001f, &m_bias);
    nu_config_get_bool("raytracer", "save_image", true, &m_saveImage);

    m_done = false;

    // Create frame resources
    m_windowInterface = std::make_unique<WindowInterface>();
    m_colorBuffer = std::make_unique<ColorFramebuffer>(m_width, m_height);

    // Create assets
    m_assetManager = std::make_unique<AssetManager>();
    const nu_vec3f_t mat0Color = {1.0f, 0.0f, 0.0f};
    const nu_vec3f_t mat1Color = {0.0f, 1.0f, 0.0f};
    const nu_vec3f_t mat2Color = {1.0f, 1.0f, 1.0f};
    MaterialHandle mat0 = m_assetManager->createMaterial(mat0Color).handle;
    MaterialHandle mat1 = m_assetManager->createMaterial(mat1Color).handle;
    MaterialHandle mat2 = m_assetManager->createMaterial(mat2Color).handle;
    
    // Create scene
    m_scene = std::make_unique<Scene>();
    
    nu_mat4f_t transform;
    nu_mat4f_identity(transform);
    m_scene->add<Sphere>(transform, mat0, 2.0f);
    
    nu_mat4f_identity(transform);
    const nu_vec3f_t translate0 = {-4.0f, 1.1f, 4.0f}; 
    nu_translate(transform, translate0);
    m_scene->add<Sphere>(transform, mat1, 1.0f);

    nu_mat4f_identity(transform);
    const nu_vec3f_t translate1 = {0.0f, 0.0f, 0.0f};
    nu_translate(transform, translate1);
    m_scene->add<Plane>(transform, mat2);

    // Create camera
    const nu_vec3f_t eye = {5.0f, 3.0f, 0.0f};
    const nu_vec3f_t center = {0.0f, 0.0f, 0.0f};
    const nu_vec3f_t up = {0.0f, 1.0f, 0.0f};
    m_camera = std::make_unique<Camera>(eye, center, up, 60.0f, (float)m_width / (float)m_height);
}

void Raytracer::render()
{
    if (!m_done) {
        // Clear buffers
        nu_vec4f_t clearColor = {0.0f, 0.0f, 0.0f, 1.0f};
        m_colorBuffer->clear(clearColor);

        // For each pixels
        #pragma omp parallel for schedule(static)
        for (uint32_t i = 0; i < m_width * m_height; i++) {
            uint32_t x = i % m_width;
            uint32_t y = i / m_width;

            nu_vec3f_t sample = {0.0f, 0.0f, 0.0f};
            for (uint32_t s = 0; s < m_sampleCount; s++) {
                float rx = m_prng.getFloat();
                float ry = m_prng.getFloat();

                float px = (float)x + rx;
                float py = (float)y + ry;

                float norm_x = (px / (float)m_width) * 2.0f - 1.0f;
                float norm_y = -((py / (float)m_height) * 2.0f - 1.0f);

                nu_vec3f_t r_pos, r_dir;
                m_camera->generateRay(norm_x, norm_y, r_pos, r_dir);

                nu_vec3f_t light;
                castRay(r_pos, r_dir, 0, light);
                nu_vec3f_add(sample, light, sample);
            }
            nu_vec3f_divs(sample, (float)m_sampleCount, sample);

            //TODO: Gamma correction

            // Save color
            nu_vec4f_t finalColor;
            nu_vec4f_from_v3(sample, 1.0f, finalColor);
            m_colorBuffer->setColor(x, y, finalColor);
        }

        m_done = true;
    }

    if (m_saveImage) {
        m_colorBuffer->save("output.jpg");
        nu_context_request_stop();
    } else {
        m_colorBuffer->display(*m_windowInterface);
    }
}

void Raytracer::castRay(const nu_vec3f_t pos, const nu_vec3f_t dir, uint32_t depth, nu_vec3f_t color) const
{
    if (depth > m_maxDepth) {
        nu_vec3f_zero(color);
        return;
    }

    HitInfo hit;
    if (m_scene->trace(pos, dir, hit)) {
        Material mat = m_assetManager->getMaterial(hit.material);

        // Light info
        nu_vec3f_t lightDir = {0.0f, -0.2f, -1.0f};
        nu_vec3f_t lightColor = {1.0f, 1.0f, 1.0f};
        float lightIntensity = 8.0f;
        nu_vec3f_normalize(lightDir);
        
        // Light dir
        nu_vec3f_t L;
        nu_vec3f_muls(lightDir, -1.0f, L);

        // Shadow ray
        nu_vec3f_t sr_pos;
        HitInfo shadowHit;
        nu_vec3f_copy(hit.normal, sr_pos);
        nu_vec3f_muls(sr_pos, m_bias, sr_pos);
        nu_vec3f_add(sr_pos, hit.position, sr_pos);
        float kShadow = m_scene->trace(sr_pos, L, shadowHit) ? 0.0f : 1.0f;

        // Direct light
        nu_vec3f_t directLight;
        float kDiffuse = NU_MAX(0.0f, nu_vec3f_dot(hit.normal, L));
        nu_vec3f_copy(lightColor, directLight);
        nu_vec3f_divs(directLight, NU_PI, directLight);
        nu_vec3f_muls(directLight, lightIntensity * kDiffuse * kShadow, directLight);

        // Indirect light
        nu_vec3f_t N, Nt, Nb;
        nu_vec3f_copy(hit.normal, N);
        createCoordinateSystem(N, Nt, Nb);

        nu_vec3f_t indirectLight = {0.0f, 0.0f, 0.0f};
        const uint32_t indirectSampleCount = 12;
        for (uint32_t n = 0; n < indirectSampleCount; n++) {
            float r1 = m_prng.getFloat();
            float r2 = m_prng.getFloat();
            nu_vec3f_t sampleLocal;
            uniformSampleHemisphere(r1, r2, sampleLocal);
            nu_vec3f_t SWDir = {
                sampleLocal[0] * Nb[0] + sampleLocal[1] * N[0] + sampleLocal[2] * Nt[0],
                sampleLocal[0] * Nb[1] + sampleLocal[1] * N[1] + sampleLocal[2] * Nt[1],
                sampleLocal[0] * Nb[2] + sampleLocal[1] * N[2] + sampleLocal[2] * Nt[2]
            };
            nu_vec3f_normalize(SWDir);
            nu_vec3f_t SWPos;
            nu_vec3f_copy(SWDir, SWPos);
            nu_vec3f_muls(SWPos, m_bias, SWPos);
            nu_vec3f_add(SWPos, hit.position, SWPos);
            
            nu_vec3f_t light;
            castRay(SWPos, SWDir, depth + 1, light);
            nu_vec3f_divs(light, 1.0f / (2.0f * NU_PI), light); // PDF
            nu_vec3f_muls(light, r1, light); // Cos correction

            nu_vec3f_add(indirectLight, light, indirectLight);
        }
        nu_vec3f_divs(indirectLight, (float)indirectSampleCount, indirectLight);

        // Final color
        nu_vec3f_copy(directLight, color);
        nu_vec3f_divs(color, NU_PI, color);
        nu_vec3f_muls(indirectLight, 0.5f, indirectLight);
        nu_vec3f_add(color, indirectLight, color);
        nu_vec3f_mul(color, mat.albedo, color);
    } else {
        nu_vec3f_zero(color);
    }
}
void Raytracer::createCoordinateSystem(const nu_vec3f_t N, nu_vec3f_t Nt, nu_vec3f_t Nb) const
{
    // Tangent
    if (fabs(N[0]) > fabs(N[1])) {
        Nt[0] = N[2];
        Nt[1] = 0.0f;
        Nt[2] = -N[0];
        nu_vec3f_divs(Nt, sqrt(N[0] * N[0] + N[2] * N[2]), Nt);
    } else {
        Nt[0] = 0.0f;
        Nt[1] = -N[2];
        Nt[2] = N[1];
        nu_vec3f_divs(Nt, sqrt(N[1] * N[1] + N[2] * N[2]), Nt);
    }

    // Bitangent
    nu_vec3f_cross(N, Nt, Nb);
}
void Raytracer::uniformSampleHemisphere(float r1, float r2, nu_vec3f_t dir) const
{
    float sinTheta = sqrtf(1.0f - r1 * r1);
    float phi = 2.0f * NU_PI * r2;
    dir[0] = sinTheta * cosf(phi);
    dir[1] = r1;
    dir[2] = sinTheta * sinf(phi);
    nu_vec3f_normalize(dir);
}