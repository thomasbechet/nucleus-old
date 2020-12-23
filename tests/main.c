#include <stdio.h>
#include <stdlib.h>

#include <nucleus/nucleus.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <nucleus/plugin/utils/module/interface.h>
static nu_result_t load_monkey(nu_renderer_mesh_handle_t *mesh)
{
    nu_module_handle_t module;
    if (nu_module_get_by_name(&module, NUUTILS_MODULE_NAME) != NU_SUCCESS) {
        nu_warning("Failed 0\n");
        return NU_SUCCESS;
    }
    nuutils_loader_interface_loader_pfn_t load_interface;
    if (nu_module_load_function(module, NUUTILS_LOADER_INTERFACE_LOADER_NAME, (nu_pfn_t*)&load_interface) != NU_SUCCESS) {
        nu_warning("Failed 1\n");
        return NU_FAILURE;
    }
    nuutils_loader_interface_t interface;
    if (load_interface(&interface) != NU_SUCCESS) {
        nu_warning("Failed 2\n");
        return NU_FAILURE;
    }

    if (interface.load_mesh_from_obj(mesh, "engine/model/alfred/alfred.obj") != NU_SUCCESS) {
        return NU_FAILURE;
    }

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
static nu_result_t on_start(void)
{
    nu_module_handle_t module;
    nu_module_load(&module, "engine/plugin/nucleus-utils");
    nu_plugin_require(module, "nuutils_command_plugin");
    nu_plugin_require(module, "nuutils_console_plugin");
    nu_plugin_require(module, "nuutils_loader_plugin");

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

    ima_data = stbi_load("engine/model/alfred/alfred.jpg", &width, &height, &channel, STBI_rgb);
    if (!ima_data) nu_interrupt("Failed to load alfred texture.\n");
    texture_info.width = (uint32_t)width;
    texture_info.height = (uint32_t)height;
    texture_info.channel = (uint32_t)channel;
    texture_info.data = ima_data;
    nu_renderer_texture_handle_t alfred_texture;
    if (nu_renderer_texture_create(&alfred_texture, &texture_info) != NU_SUCCESS) {
        nu_warning("Failed to create texture.\n");
    }
    stbi_image_free(ima_data);

    /* create materials */
    nu_renderer_material_create_info_t material_info;
    material_info.diffuse_texture      = rdr2_texture_id;
    material_info.use_diffuse_uniform  = false;
    material_info.normal_texture       = NU_NULL_HANDLE;
    material_info.use_normal_uniform   = false;
    material_info.specular_texture     = NU_NULL_HANDLE;
    material_info.use_specular_uniform = false;

    nu_renderer_material_handle_t material0 = {0};
    if (nu_renderer_material_create(&material0, &material_info) != NU_SUCCESS) {
        nu_interrupt("Failed to create material0.\n");
    }

    nu_renderer_material_handle_t material1 = {0};
    material_info.diffuse_texture = brick_texture_id;
    if (nu_renderer_material_create(&material1, &material_info) != NU_SUCCESS) {
        nu_interrupt("Failed to create material1.\n");
    }

    nu_renderer_material_handle_t alfred_material = {0};
    material_info.diffuse_texture = alfred_texture;
    if (nu_renderer_material_create(&alfred_material, &material_info) != NU_SUCCESS) {
        nu_interrupt("Failed to create alfred_material.\n");
    }

    /* load cube mesh */
    nu_vec3_t positions[] =
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
    uint32_t position_indices[] =
    {
        0, 3, 1, 1, 3, 2, /* back   */
        1, 2, 5, 5, 2, 6, /* right  */
        5, 6, 4, 4, 6, 7, /* front  */
        4, 7, 0, 0, 7, 3, /* left   */
        3, 7, 2, 2, 7, 6, /* top    */
        4, 0, 5, 5, 0, 1  /* bottom */
    };
    nu_vec2_t uvs[] =
    {
        {0, 0},
        {1, 0},
        {1, 1},
        {0, 1}
    };
    uint32_t uv_indices[] =
    {
        1, 2, 0, 0, 2, 3, /* back   */
        1, 2, 0, 0, 2, 3, /* right  */
        1, 2, 0, 0, 2, 3, /* front  */
        1, 2, 0, 0, 2, 3, /* left   */
        3, 0, 2, 2, 0, 1, /* top    */
        3, 0, 2, 2, 0, 1  /* bottom */
    };
    uint32_t indice_count = 6 * 6;

    nu_renderer_mesh_create_info_t mesh_info;
    memset(&mesh_info, 0, sizeof(nu_renderer_mesh_create_info_t));
    mesh_info.indice_count     = indice_count;
    mesh_info.positions        = positions;
    mesh_info.uvs              = uvs;
    mesh_info.position_indices = position_indices;
    mesh_info.uv_indices       = uv_indices;

    nu_renderer_mesh_handle_t mesh_id;
    if (nu_renderer_mesh_create(&mesh_id, &mesh_info) != NU_SUCCESS) {
        nu_warning("Failed to create cube mesh.\n");
    }

    /* create static meshes */
    nu_renderer_staticmesh_handle_t staticmesh_id;
    nu_renderer_staticmesh_create_info_t staticmesh_info = {0};
    staticmesh_info.mesh = mesh_id;
    staticmesh_info.material = material0;

    nu_mat4_identity(staticmesh_info.transform);
    nu_translate(staticmesh_info.transform, (nu_vec3_t){0, -4, 0});
    nu_scale(staticmesh_info.transform, (nu_vec3_t){100.0, 0.1, 100.0});
    nu_renderer_staticmesh_create(&staticmesh_id, &staticmesh_info);

    staticmesh_info.material = material1;

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

    /* create monkey */
    nu_renderer_mesh_handle_t mesh_handle;
    if (load_monkey(&mesh_handle) == NU_SUCCESS) {
        staticmesh_info.mesh = mesh_handle;
        staticmesh_info.material = alfred_material;
        nu_mat4_identity(staticmesh_info.transform);
        nu_translate(staticmesh_info.transform, (nu_vec3_t){0, 30, 0});
        nu_scale(staticmesh_info.transform, (nu_vec3_t){4, 4, 4});
        nu_renderer_staticmesh_create(&staticmesh_id, &staticmesh_info);
    } else {
        nu_warning("Failed to load monkey\n");
    }
}
static nu_result_t on_stop(void)
{

}
static nu_result_t on_update(void)
{
    /* quit handling */
    nu_button_state_t escape_state;
    nu_input_get_keyboard_state(&escape_state, NU_KEYBOARD_ESCAPE);
    if (escape_state & NU_BUTTON_PRESSED) {
        nu_context_request_stop();
    }

    nu_cursor_mode_t cursor_mode;
    nu_input_get_cursor_mode(&cursor_mode);
    if (cursor_mode & NU_CURSOR_MODE_DISABLE) {
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
        nu_button_state_t z_state, s_state, q_state, d_state, shift_state;
        nu_input_get_keyboard_state(&z_state, NU_KEYBOARD_W);
        nu_input_get_keyboard_state(&s_state, NU_KEYBOARD_S);
        nu_input_get_keyboard_state(&q_state, NU_KEYBOARD_A);
        nu_input_get_keyboard_state(&d_state, NU_KEYBOARD_D);
        nu_input_get_keyboard_state(&shift_state, NU_KEYBOARD_LSHIT);
        if (z_state & NU_BUTTON_PRESSED) move[2] -= 1;
        if (s_state & NU_BUTTON_PRESSED) move[2] += 1;
        if (q_state & NU_BUTTON_PRESSED) move[0] -= 1;
        if (d_state & NU_BUTTON_PRESSED) move[0] += 1;
        nu_mat4_mulv3(camera, move, 0, move);
        nu_vec3_normalize(move);
        if (shift_state & NU_BUTTON_PRESSED) {
            nu_vec3_muls(move, 0.04 * nu_context_get_delta_time(), move);
        } else {
            nu_vec3_muls(move, 0.01 * nu_context_get_delta_time(), move);
        }
        nu_vec3_add(eye, move, eye);
        
        nu_vec3_t center;
        nu_vec3_add(eye, forward, center);
        nu_renderer_camera_set_eye(NULL, eye);
        nu_renderer_camera_set_center(NULL, center);
        //nu_renderer_camera_set_fov(NULL, nu_radian(90.0));
    }
}

int main(int argc, char *argv[])
{
    nu_context_init_info_t info;
    memset(&info, 0, sizeof(nu_context_init_info_t));
    info.callback.start  = on_start;
    info.callback.stop   = on_stop;
    info.callback.update = on_update;
    nu_context_init(&info);

    return 0;
}