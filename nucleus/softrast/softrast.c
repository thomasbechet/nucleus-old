#include "softrast.h"

#include "common/logger.h"
#include "common/config.h"
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
    uint32_t default_width, default_height;
    nu_config_get_uint(NUSR_CONFIG_SOFTRAST_SECTION, NUSR_CONFIG_SOFTRAST_FRAMEBUFFER_WIDTH, &default_width, 640);
    nu_config_get_uint(NUSR_CONFIG_SOFTRAST_SECTION, NUSR_CONFIG_SOFTRAST_FRAMEBUFFER_HEIGHT, &default_height, 360);
    nusr_framebuffer_create(&_data.framebuffer, default_width, default_height);

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

static void profile(void)
{
    static float avg = 0.0;
    static uint32_t avg_count = 0;
    float delta = nu_context_get_delta_time();
    avg += delta;
    avg_count++;
    if (avg_count > 50) {
        nu_info("delta %f\n", avg / (float)avg_count);
        avg = 0.0;
        avg_count = 0;
    }
}

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

    uint32_t staticmesh_id;
    nusr_staticmesh_create_info_t staticmesh_info = {};
    staticmesh_info.mesh = mesh_id;
    glm_mat4_identity(staticmesh_info.transform);
    nusr_scene_staticmesh_create(&staticmesh_id, &staticmesh_info);

    glm_mat4_identity(staticmesh_info.transform);
    glm_translate(staticmesh_info.transform, (vec3){3, 0, 0});
    glm_scale(staticmesh_info.transform, (vec3){0.5f, 0.5f, 0.5f});
    nusr_scene_staticmesh_create(&staticmesh_id, &staticmesh_info);

    glm_mat4_identity(staticmesh_info.transform);
    glm_translate(staticmesh_info.transform, (vec3){-3, 0, 0});
    glm_scale(staticmesh_info.transform, (vec3){0.2, 2, 0.5});
    nusr_scene_staticmesh_create(&staticmesh_id, &staticmesh_info);
}
static void test_update(void)
{
    /* quit handling */
    nu_button_state_t escape_state;
    nu_input_get_keyboard_state(&escape_state, NU_KEYBOARD_ESCAPE);
    if (escape_state == NU_BUTTON_PRESSED) {
        nu_context_request_stop();
    }

    /* camera update */
    vec2 motion;
    nu_input_get_mouse_motion(motion);
    static float yaw = 0.0f;
    static float pitch = 0.0f;
    yaw += motion[0] * 0.3f;
    pitch -= motion[1] * 0.3f;
    if (pitch < -90.0f) pitch = -90.0f;
    if (pitch > 90.0f) pitch = 90.0f;
    static mat4 camera;
    glm_mat4_identity(camera);
    glm_rotate(camera, -glm_rad(yaw), (vec3){0, 1, 0});
    glm_rotate(camera, glm_rad(pitch), (vec3){1, 0, 0});
    vec3 forward;
    glm_mat4_mulv3(camera, (vec3){0, 0, -1}, 0, forward);
    glm_normalize(forward);

    static vec3 eye = {0, 0, 5};
    vec3 move;
    glm_vec3_fill(move, 0);
    nu_button_state_t z_state, s_state, q_state, d_state;
    nu_input_get_keyboard_state(&z_state, NU_KEYBOARD_W);
    nu_input_get_keyboard_state(&s_state, NU_KEYBOARD_S);
    nu_input_get_keyboard_state(&q_state, NU_KEYBOARD_A);
    nu_input_get_keyboard_state(&d_state, NU_KEYBOARD_D);
    if (z_state == NU_BUTTON_PRESSED) move[2] -= 1;
    if (s_state == NU_BUTTON_PRESSED) move[2] += 1;
    if (q_state == NU_BUTTON_PRESSED) move[0] -= 1;
    if (d_state == NU_BUTTON_PRESSED) move[0] += 1;
    glm_mat4_mulv3(camera, move, 0, move);
    glm_normalize(move);
    glm_vec3_scale(move, 0.01 * nu_context_get_delta_time(), move);
    glm_vec3_add(eye, move, eye);
    
    vec3 center;
    glm_vec3_add(eye, forward, center);
    nusr_scene_camera_set_eye(eye);
    nusr_scene_camera_set_center(center);
    nusr_scene_camera_set_fov(glm_rad(90.0));
}
