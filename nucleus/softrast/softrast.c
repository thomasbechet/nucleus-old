#include "softrast.h"

#include "common/logger.h"
#include "memory/framebuffer.h"

#include "../glfw/module/interface.h"

#include <cglm/cglm.h>

typedef struct {
    nuglfw_window_interface_t glfw_interface;
    nusr_framebuffer_t framebuffer;
} nusr_data_t;

static nusr_data_t _data;

static void profile(void);

static nu_result_t load_glfw_interface(void)
{
    const nu_module_t *window_module = nu_system_window_get_module();
    if (window_module->info.id != NUGLFW_MODULE_ID) {
        nu_fatal(NUSR_LOGGER_NAME"Software rasterizer requires GLFW module to work.\n");
        return NU_FAILURE;
    }

    nuglfw_window_interface_loader_pfn_t load_interface;
    if (nu_module_load_function(window_module, NUGLFW_WINDOW_INTERFACE_LOADER_NAME, (nu_pfn_t*)&load_interface) != NU_SUCCESS) {
        nu_warning(NUSR_LOGGER_NAME"Software rasterizer failed to load glfw loader.\n");
        return NU_FAILURE;
    }

    if (load_interface(&_data.glfw_interface) != NU_SUCCESS) {
        nu_warning(NUSR_LOGGER_NAME"Software rasterizer failed to load glfw interface.\n");
        return NU_FAILURE;
    }

    return NU_SUCCESS;
}

static void project_vertex(const vec3 v, vec2 out)
{
    const float near = 0.1f;
    const float l = -1;
    const float r = 1;
    const float b = -1;
    const float t = 1;

    /* convert to screen space */
    vec2 pscreen = {
        (near * v[0]) / v[2],
        (near * v[1]) / v[2]
    };

    /* convert to NDC space */
    vec2 pNDC = {
        2 * pscreen[0] / (r - l) - (r + l) / (r - l),
        2 * pscreen[1] / (t - b) - (t + b) / (t - b)
    };

    /* convert to raster space */
    out[0] = (pscreen[0] + 1) / 2 * _data.framebuffer.width;
    out[1] = (1 - pscreen[1]) / 2 * _data.framebuffer.height;
}

static float edge_function(const vec2 a, const vec2 b, const vec2 c)
{
    return (c[0] - a[0]) * (b[1] - a[1]) - (c[1] - a[1]) * (b[0] - a[0]);
}

nu_result_t nusr_initialize(void)
{
    /* loading glfw interface */
    nu_info(NUSR_LOGGER_NAME"Loading glfw interface...\n");
    if (load_glfw_interface() != NU_SUCCESS) {
        nu_warning(NUSR_LOGGER_NAME"Failed to load glfw interface.\n");
        return NU_FAILURE;
    }

    /* creating framebuffer */
    nu_info(NUSR_LOGGER_NAME"Creating framebuffer...\n");
    nusr_framebuffer_create(&_data.framebuffer, 640, 360);
    //nusr_framebuffer_create(&_data.framebuffer, 1920, 1080);
    //nusr_framebuffer_create(&_data.framebuffer, 256, 144);

    return NU_SUCCESS;
}
nu_result_t nusr_terminate(void)
{
    /* free framebuffer */
    nusr_framebuffer_destroy(&_data.framebuffer);

    return NU_SUCCESS;
}
nu_result_t nusr_render(void)
{
    mat4 camera_mat4;
    mat4 camera_mat4_inv;
    glm_lookat((vec3){1.0f, 1.0f, 0.0f}, (vec3){0.0f, 0.0f, 0.0f}, (vec3){0.0f, 1.0f, 0.0f}, camera_mat4);
    glm_mat4_inv(camera_mat4, camera_mat4_inv);

    static const vec3 vertices[8] =
    {
        {-1, -1, -1},
        {1, -1, -1},
        {1, 1, -1},
        {-1, 1, -1},
        {-1, -1, 1},
        {1, -1, 1},
        {1, 1, 1},
        {-1, 1, 1}
    };
    static const vec2 uvs[4] =
    {
        {0, 0},
        {1, 0},
        {1, 1},
        {0, 1}
    };
    static const uint32_t indices[6 * 6] =
    {
        0, 1, 3, 3, 1, 2,
        1, 5, 2, 2, 5, 6,
        5, 4, 6, 6, 4, 7,
        4, 0, 7, 7, 0, 3,
        3, 2, 7, 7, 2, 6,
        4, 5, 0, 0, 5, 1
    };
    static const uint32_t vcount = 6 * 6;

    nusr_framebuffer_clear(&_data.framebuffer, 0x0);

    for (uint32_t vi = 0; vi < vcount; vi += 3) {

        vec2 v0, v1, v2;

        /* transform vertices */
        vec4 tv0, tv1, tv2;
        glm_vec4((float*)vertices[indices[vi + 0]], 1.0f, tv0);
        glm_vec4((float*)vertices[indices[vi + 1]], 1.0f, tv1);
        glm_vec4((float*)vertices[indices[vi + 2]], 1.0f, tv2);
        glm_mat4_mulv(camera_mat4_inv, tv0, tv0);
        glm_mat4_mulv(camera_mat4_inv, tv1, tv1);
        glm_mat4_mulv(camera_mat4_inv, tv2, tv2);

        /* project vertices */
        project_vertex((float*)tv0, v0);
        project_vertex((float*)tv1, v1);
        project_vertex((float*)tv2, v2);

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

                    nusr_framebuffer_set_rgb(&_data.framebuffer, i, j, r, g, b);
                }
            }
        }
    }

    _data.glfw_interface.present_surface(_data.framebuffer.width, _data.framebuffer.height, _data.framebuffer.pixels);
    
    profile();

    return NU_SUCCESS;
}

#include <Windows.h>
static void profile(void)
{
    static LARGE_INTEGER frequency;
    static LARGE_INTEGER t1, t2;
    static double delta;
    static double avg = 0.0;
    static uint32_t avg_count = 0;
    QueryPerformanceCounter(&t2);
    QueryPerformanceFrequency(&frequency);
    delta = (t2.QuadPart - t1.QuadPart) * 1000.0 / frequency.QuadPart;
    avg += delta;
    avg_count++;
    if (avg_count > 50) {
        nu_info("delta %lf\n", avg / (double)avg_count);
        avg = 0.0;
        avg_count = 0;
    }
    QueryPerformanceCounter(&t1);
}