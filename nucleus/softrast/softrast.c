#include "softrast.h"

#include "common/logger.h"
#include "memory/framebuffer.h"
#include "scene/scene.h"
#include "asset/mesh.h"

#include "../glfw/module/interface.h"

typedef struct {
    nuglfw_window_interface_t glfw_interface;
    nusr_framebuffer_t framebuffer;
} nusr_data_t;

static nusr_data_t _data;

static void profile(void);
static void test_initialize(void);
static void test_update(void);

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

nu_result_t nusr_initialize(void)
{
    /* loading glfw interface */
    nu_info(NUSR_LOGGER_NAME"Loading glfw interface...\n");
    if (load_glfw_interface() != NU_SUCCESS) {
        nu_warning(NUSR_LOGGER_NAME"Failed to load glfw interface.\n");
        return NU_FAILURE;
    }

    /* initialize assets */
    nu_info(NUSR_LOGGER_NAME"Initializing asset buffers...\n");
    nusr_mesh_initialize();

    /* initialize scene */
    nu_info(NUSR_LOGGER_NAME"Intializing scene memory...\n");
    nusr_scene_initialize();

    /* creating framebuffer */
    nu_info(NUSR_LOGGER_NAME"Creating framebuffer...\n");
    nusr_framebuffer_create(&_data.framebuffer, 640, 360);
    //nusr_framebuffer_create(&_data.framebuffer, 1920, 1080);
    //nusr_framebuffer_create(&_data.framebuffer, 256, 144);

    test_initialize();

    return NU_SUCCESS;
}
nu_result_t nusr_terminate(void)
{
    /* free framebuffer */
    nusr_framebuffer_destroy(&_data.framebuffer);

    /* terminate scene */
    nusr_scene_terminate();

    /* terminate assets */
    nusr_mesh_terminate();

    return NU_SUCCESS;
}
nu_result_t nusr_render(void)
{
    test_update();

    nusr_framebuffer_clear(&_data.framebuffer, 0x0);
    nusr_scene_render(&_data.framebuffer);
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

uint32_t staticmesh_id;
static void test_initialize(void)
{
    static vec3 vertices[] =
    {
        {-1, -1, -1},
        { 1, -1, -1},
        { 1,  1, -1},
        {-1,  1, -1},
        {-1, -1,  1},
        { 1, -1,  1},
        { 1,  1,  1},
        {-1,  1,  1}
    };
    static vec2 uvs[] =
    {
        {0, 0},
        {1, 0},
        {1, 1},
        {0, 1}
    };
    static vec3 colors[] = {
        {1, 1, 1},
        {1, 1, 1},
        {1, 1, 1},
        {1, 1, 1},
        {1, 1, 1},
        {1, 1, 1},
        {1, 1, 1},
        {1, 1, 1}
    };
    static uint32_t indices[] =
    {
        0, 1, 3, 3, 1, 2,
        1, 5, 2, 2, 5, 6,
        5, 4, 6, 6, 4, 7,
        4, 0, 7, 7, 0, 3,
        3, 2, 7, 7, 2, 6,
        4, 5, 0, 0, 5, 1
    };
    static uint32_t vcount = 6 * 6;

    nusr_mesh_create_info_t mesh_info = {};
    mesh_info.vertice_count = vcount;
    mesh_info.positions = vertices;
    mesh_info.uvs = uvs;
    mesh_info.colors = colors;
    mesh_info.indices = indices;
    uint32_t mesh_id;
    if (nusr_mesh_create(&mesh_id, &mesh_info) != NU_SUCCESS) {
        nu_warning("Failed to create mesh.\n");
    }

    nusr_staticmesh_create_info_t staticmesh_info = {};
    staticmesh_info.mesh = mesh_id;
    glm_mat4_identity(staticmesh_info.transform);
    
    if (nusr_scene_staticmesh_create(&staticmesh_id, &staticmesh_info) != NU_SUCCESS) {
        nu_warning("Failed to create staticmesh.\n");
    }

    glm_translate(staticmesh_info.transform, (vec3){2.5f, 0.0f, 0.0f});
    glm_scale(staticmesh_info.transform, (vec3){0.5f, 0.5f, 0.5f});
    if (nusr_scene_staticmesh_create(&staticmesh_id, &staticmesh_info) != NU_SUCCESS) {
        nu_warning("Failed to create staticmesh.\n");
    }

    nusr_scene_camera_set_eye((vec3){3, 3, 3});
    nusr_scene_camera_set_fov(70.0);
}
static void test_update(void)
{
    static float angle = 0.0f;
    angle += 0.001f;
    mat4 m;
    glm_mat4_identity(m);
    glm_rotate(m, angle, (vec3){0.0, 1.0, 0.0});
    glm_translate(m, (vec3){2.5, 0.0, 0.0});
    glm_scale(m, (vec3){0.5, 0.5, 0.5});
    nusr_scene_staticmesh_set_transform(staticmesh_id, m);
}
