#include "softrast.h"

#include "common/logger.h"
#include "common/config.h"
#include "viewport/viewport.h"
#include "scene/scene.h"
#include "gui/gui.h"
#include "asset/mesh.h"
#include "asset/texture.h"
#include "asset/font.h"

#include "../glfw/module/interface.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

typedef struct {
    nuglfw_window_interface_t glfw_interface;
} nusr_data_t;

static nusr_data_t _data;

static void profile(void);
static void test_initialize(void);
static void test_update(void);

static nu_result_t load_glfw_interface(void)
{
    nu_module_handle_t window_module = nu_system_window_get_module_handle();
    if (nu_module_get_id(window_module) != NUGLFW_MODULE_ID) {
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
    nu_info(NUSR_LOGGER_NAME"Initializing assets...\n");
    if (nusr_mesh_initialize() != NU_SUCCESS) return NU_FAILURE;
    if (nusr_texture_initialize() != NU_SUCCESS) return NU_FAILURE;
    if (nusr_font_initialize() != NU_SUCCESS) return NU_FAILURE;

    /* initialize viewport */
    nu_info(NUSR_LOGGER_NAME"Initializing viewport...\n");
    nusr_viewport_initialize();

    /* initialize scene */
    nu_info(NUSR_LOGGER_NAME"Intializing scene...\n");
    nusr_scene_initialize();

    /* initialize gui */
    nu_info(NUSR_LOGGER_NAME"Initialize gui...\n");
    nusr_gui_initialize();

    test_initialize();

    return NU_SUCCESS;
}
nu_result_t nusr_terminate(void)
{
    /* free framebuffer */
    nusr_framebuffer_destroy(&_data.color_buffer);
    nusr_framebuffer_destroy(&_data.depth_buffer);

    /* terminate gui */
    nu_info(NUSR_LOGGER_NAME"Terminating gui...\n");
    nusr_gui_terminate();

    /* terminate scene */
    nu_info(NUSR_LOGGER_NAME"Terminating scene...\n");
    nusr_scene_terminate();

    /* terminate assets */
    nu_info(NUSR_LOGGER_NAME"Terminating assets...\n");
    nusr_font_terminate();
    nusr_texture_terminate();
    nusr_mesh_terminate();

    return NU_SUCCESS;
}
nu_result_t nusr_render(void)
{
    test_update();

    nusr_scene_render(&_data.color_buffer, &_data.depth_buffer);
    nusr_gui_render(&_data.color_buffer);
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
    nu_renderer_texture_create_info_t texture_info = {};
    int width, height, channel;
    unsigned char *ima_data;

    ima_data = stbi_load("engine/texture/brick.jpg", &width, &height, &channel, STBI_rgb);
    if (!ima_data) nu_interrupt("Failed to load brick texture.\n");
    texture_info.width = (uint32_t)width;
    texture_info.height = (uint32_t)height;
    texture_info.channel = (uint32_t)channel;
    texture_info.data = ima_data;
    nu_renderer_texture_handle_t brick_texture_id;
    if (nu_renderer_texture_create(&brick_texture_id, &texture_info) != NU_SUCCESS) {
        nu_warning("Failed to create texture.\n");
    }
    stbi_image_free(ima_data);

    ima_data = stbi_load("engine/texture/checkerboard.jpg", &width, &height, &channel, STBI_rgb);
    if (!ima_data) nu_interrupt("Failed to load rdr2 texture.\n");
    texture_info.width = (uint32_t)width;
    texture_info.height = (uint32_t)height;
    texture_info.channel = (uint32_t)channel;
    texture_info.data = ima_data;
    nu_renderer_texture_handle_t rdr2_texture_id;
    if (nu_renderer_texture_create(&rdr2_texture_id, &texture_info) != NU_SUCCESS) {
        nu_warning("Failed to create texture.\n");
    }
    stbi_image_free(ima_data);

    /* load cube mesh */
    static nu_vec3_t vertices[] =
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
    static nu_vec2_t uvs[] =
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

    nu_renderer_mesh_create_info_t mesh_info = {};
    mesh_info.vertice_count = vcount;
    mesh_info.use_indices = true;
    mesh_info.use_colors = false;
    mesh_info.positions = vertices;
    mesh_info.uvs = uvs;
    mesh_info.position_indices = position_indices;
    mesh_info.uv_indices = uv_indices;
    nu_renderer_mesh_handle_t mesh_id;
    if (nu_renderer_mesh_create(&mesh_id, &mesh_info) != NU_SUCCESS) {
        nu_warning("Failed to create cube mesh.\n");
    }

    /* load triangle mesh */
    static nu_vec3_t triangle_vertices[] = {
        {0, 0, 0},
        {10, 0, 0},
        {0, 0, 10}
    };
    static nu_vec2_t triangle_uvs[] = {
        {0, 1},
        {1, 1},
        {0, 0}
    };

    nu_renderer_mesh_create_info_t triangle_mesh_info = {};
    triangle_mesh_info.vertice_count = 3;
    triangle_mesh_info.use_indices = false;
    triangle_mesh_info.use_colors = false;
    triangle_mesh_info.positions = triangle_vertices;
    triangle_mesh_info.uvs = triangle_uvs;
    triangle_mesh_info.position_indices = NULL;
    triangle_mesh_info.uv_indices = NULL;
    nu_renderer_mesh_handle_t triangle_mesh_id;
    if (nu_renderer_mesh_create(&triangle_mesh_id, &triangle_mesh_info) != NU_SUCCESS) {
        nu_warning("Failed to create triangle mesh.\n");
    }

    /* create static meshes */
    nu_renderer_staticmesh_handle_t staticmesh_id;
    nu_renderer_staticmesh_create_info_t staticmesh_info = {};
    staticmesh_info.mesh = mesh_id;
    staticmesh_info.texture = rdr2_texture_id;

    nu_mat4_identity(staticmesh_info.transform);
    nu_translate(staticmesh_info.transform, (nu_vec3_t){0, -4, 0});
    nu_scale(staticmesh_info.transform, (nu_vec3_t){100.0, 0.1, 100.0});
    nu_renderer_staticmesh_create(&staticmesh_id, &staticmesh_info);

    staticmesh_info.texture = brick_texture_id;

    for (uint32_t i = 0; i < 5; i++) {
        for (uint32_t j = 0; j < 5; j++) {
            for (uint32_t k = 0; k < 5; k++) {
                nu_mat4_identity(staticmesh_info.transform);
                nu_translate(staticmesh_info.transform, (nu_vec3_t){i * 2, k * 2, j * 2});
                nu_scale(staticmesh_info.transform, (nu_vec3_t){0.5, 0.5, 0.5});
                nu_renderer_staticmesh_create(&staticmesh_id, &staticmesh_info);
            }
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

    nu_button_state_t f1_state, f2_state;
    nu_input_get_keyboard_state(&f1_state, NU_KEYBOARD_F1);
    nu_input_get_keyboard_state(&f2_state, NU_KEYBOARD_F2);
    if (f1_state == NU_BUTTON_PRESSED) {
        nu_input_set_cursor_mode(NU_CURSOR_MODE_NORMAL);
    } else if (f2_state == NU_BUTTON_PRESSED) {
        nu_input_set_cursor_mode(NU_CURSOR_MODE_DISABLE);
    }

    nu_cursor_mode_t cursor_mode;
    nu_input_get_cursor_mode(&cursor_mode);
    if (cursor_mode == NU_CURSOR_MODE_DISABLE) {
        /* camera update */
        nu_vec2_t motion;
        nu_input_get_mouse_motion(motion);
        static float yaw = 0.0f;
        static float pitch = 0.0f;
        yaw += motion[0] * 0.3f;
        pitch -= motion[1] * 0.3f;
        if (pitch <= -90.0f) pitch = -89.999f;
        if (pitch >= 90.0f) pitch = 89.999f;
        static nu_mat4_t camera;
        nu_mat4_identity(camera);
        nu_rotate(camera, -nu_radian(yaw), (nu_vec3_t){0, 1, 0});
        nu_rotate(camera, nu_radian(pitch), (nu_vec3_t){1, 0, 0});
        nu_vec3_t forward;
        nu_mat4_mulv3(camera, (nu_vec3_t){0, 0, -1}, 0, forward);
        nu_vec3_normalize(forward);

        static nu_vec3_t eye = {0, 0, 5};
        nu_vec3_t move;
        nu_vec3_zero(move);
        nu_button_state_t z_state, s_state, q_state, d_state;
        nu_input_get_keyboard_state(&z_state, NU_KEYBOARD_W);
        nu_input_get_keyboard_state(&s_state, NU_KEYBOARD_S);
        nu_input_get_keyboard_state(&q_state, NU_KEYBOARD_A);
        nu_input_get_keyboard_state(&d_state, NU_KEYBOARD_D);
        if (z_state == NU_BUTTON_PRESSED) move[2] -= 1;
        if (s_state == NU_BUTTON_PRESSED) move[2] += 1;
        if (q_state == NU_BUTTON_PRESSED) move[0] -= 1;
        if (d_state == NU_BUTTON_PRESSED) move[0] += 1;
        nu_mat4_mulv3(camera, move, 0, move);
        nu_vec3_normalize(move);
        nu_vec3_muls(move, 0.01 * nu_context_get_delta_time(), move);
        nu_vec3_add(eye, move, eye);
        
        nu_vec3_t center;
        nu_vec3_add(eye, forward, center);
        nu_renderer_camera_set_eye(NULL, eye);
        nu_renderer_camera_set_center(NULL, center);
        nu_renderer_camera_set_fov(NULL, nu_radian(90.0));
    }
}
