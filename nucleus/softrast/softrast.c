#include "softrast.h"

#include "common/logger.h"
#include "common/config.h"
#include "memory/framebuffer.h"
#include "scene/scene.h"
#include "asset/mesh.h"
#include "asset/texture.h"

#include "../glfw/module/interface.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

typedef struct {
    nuglfw_window_interface_t glfw_interface;
    nusr_framebuffer_t color_buffer;
    nusr_framebuffer_t depth_buffer;
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
    nusr_texture_initialize();

    /* initialize scene */
    nu_info(NUSR_LOGGER_NAME"Intializing scene memory...\n");
    nusr_scene_initialize();

    /* creating framebuffer */
    nu_info(NUSR_LOGGER_NAME"Creating framebuffer...\n");
    uint32_t default_width, default_height;
    nu_config_get_uint(NUSR_CONFIG_SOFTRAST_SECTION, NUSR_CONFIG_SOFTRAST_FRAMEBUFFER_WIDTH, &default_width, 640);
    nu_config_get_uint(NUSR_CONFIG_SOFTRAST_SECTION, NUSR_CONFIG_SOFTRAST_FRAMEBUFFER_HEIGHT, &default_height, 360);
    nusr_framebuffer_create(&_data.color_buffer, default_width, default_height);
    nusr_framebuffer_create(&_data.depth_buffer, default_width, default_height);

    test_initialize();

    return NU_SUCCESS;
}
nu_result_t nusr_terminate(void)
{
    /* free framebuffer */
    nusr_framebuffer_destroy(&_data.color_buffer);
    nusr_framebuffer_destroy(&_data.depth_buffer);

    /* terminate scene */
    nusr_scene_terminate();

    /* terminate assets */
    nusr_texture_terminate();
    nusr_mesh_terminate();

    return NU_SUCCESS;
}
nu_result_t nusr_render(void)
{
    test_update();

    nusr_scene_render(&_data.color_buffer, &_data.depth_buffer);
    _data.glfw_interface.present_surface(_data.color_buffer.width, _data.color_buffer.height, _data.color_buffer.pixels);
    
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
    /* load texture */
    nusr_texture_create_info_t texture_info = {};
    int width, height, channel;
    unsigned char *ima_data = stbi_load("engine/image/rdr2.jpg", &width, &height, &channel, STBI_rgb);
    if (!ima_data) nu_interrupt("Failed to load brick texture.\n");
    texture_info.width = (uint32_t)width;
    texture_info.height = (uint32_t)height;
    texture_info.channel = (uint32_t)channel;
    texture_info.data = ima_data;
    uint32_t texture_id;
    if (nusr_texture_create(&texture_id, &texture_info) != NU_SUCCESS) {
        nu_warning("Failed to create texture.\n");
    }
    stbi_image_free(ima_data);

    /* load cube mesh */
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
    static uint32_t position_indices[] =
    {
        0, 1, 3, 3, 1, 2, /* back */
        1, 5, 2, 2, 5, 6, /* right */
        5, 4, 6, 6, 4, 7, /* front */
        4, 0, 7, 7, 0, 3, /* left */
        3, 2, 7, 7, 2, 6, /* top */
        4, 5, 0, 0, 5, 1  /* bottom */
    };
    static vec2 uvs[] =
    {
        {0, 0},
        {1, 0},
        {1, 1},
        {0, 1}
    };
    static uint32_t uv_indices[] =
    {
        1, 0, 2, 2, 0, 3, /* back */
        1, 0, 2, 2, 0, 3, /* right */
        1, 0, 2, 2, 0, 3, /* front */
        1, 0, 2, 2, 0, 3, /* left */
        3, 2, 0, 0, 2, 1, /* top */
        3, 2, 0, 0, 2, 1 /* bottom */
    };
    
    static uint32_t vcount = 6 * 6;

    nusr_mesh_create_info_t mesh_info = {};
    mesh_info.vertice_count = vcount;
    mesh_info.use_indices = true;
    mesh_info.use_colors = false;
    mesh_info.positions = vertices;
    mesh_info.uvs = uvs;
    mesh_info.position_indices = position_indices;
    mesh_info.uv_indices = uv_indices;
    uint32_t mesh_id;
    if (nusr_mesh_create(&mesh_id, &mesh_info) != NU_SUCCESS) {
        nu_warning("Failed to create cube mesh.\n");
    }

    /* load triangle mesh */
    static vec3 triangle_vertices[] = {
        {0, 0, 0},
        {10, 0, 0},
        {0, 0, 10}
    };
    static vec2 triangle_uvs[] = {
        {0, 1},
        {1, 1},
        {0, 0}
    };

    nusr_mesh_create_info_t triangle_mesh_info = {};
    triangle_mesh_info.vertice_count = 3;
    triangle_mesh_info.use_indices = false;
    triangle_mesh_info.use_colors = false;
    triangle_mesh_info.positions = triangle_vertices;
    triangle_mesh_info.uvs = triangle_uvs;
    triangle_mesh_info.position_indices = NULL;
    triangle_mesh_info.uv_indices = NULL;
    uint32_t triangle_mesh_id;
    if (nusr_mesh_create(&triangle_mesh_id, &triangle_mesh_info) != NU_SUCCESS) {
        nu_warning("Failed to create triangle mesh.\n");
    }

    /* create triangle mesh */
    // uint32_t triangle_staticmesh;
    // nusr_staticmesh_create_info_t tri_create_info = {};
    // tri_create_info.mesh = triangle_mesh_id;
    // tri_create_info.texture = texture_id;
    // glm_mat4_identity(tri_create_info.transform);
    // glm_translate(tri_create_info.transform, (vec3){0, 0, 0});
    // nusr_scene_staticmesh_create(&triangle_staticmesh, &tri_create_info);

    /* create static meshes */
    uint32_t staticmesh_id;
    nusr_staticmesh_create_info_t staticmesh_info = {};
    staticmesh_info.mesh = mesh_id;
    staticmesh_info.texture = texture_id;

    // glm_mat4_identity(staticmesh_info.transform);
    // glm_translate(staticmesh_info.transform, (vec3){0, -4, 0});
    // glm_scale(staticmesh_info.transform, (vec3){100.0, 0.1, 100.0});
    // nusr_scene_staticmesh_create(&staticmesh_id, &staticmesh_info);

    for (uint32_t i = 0; i < 10; i++) {
        for (uint32_t j = 0; j < 10; j++) {
            glm_mat4_identity(staticmesh_info.transform);
            glm_translate(staticmesh_info.transform, (vec3){i * 3, 0, j * 3});
            glm_scale(staticmesh_info.transform, (vec3){0.5, 0.5, 0.5});
            nusr_scene_staticmesh_create(&staticmesh_id, &staticmesh_info);
        }
    }
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
