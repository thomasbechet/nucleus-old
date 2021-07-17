#include <cstdio>
#include <cstdlib>
#include <iostream>

#include <nucleus/nucleus.hpp>
#include <nucleus/module/utils.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

static nu_result_t load_monkey(nu_renderer_mesh_handle_t *mesh)
{
    nu_module_handle_t module;
    nuutils_loader_interface_t loader;
    if (nu_module_get_by_name(&module, NUUTILS_MODULE_NAME) != NU_SUCCESS) return NU_FAILURE;
    if (nu_module_load_interface(module, NUUTILS_LOADER_INTERFACE_NAME, &loader) != NU_SUCCESS) return NU_FAILURE;
    if (loader.load_mesh_from_obj(mesh, "engine/model/alfred/alfred.obj") != NU_SUCCESS) return NU_FAILURE;

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
typedef struct {
    uint32_t id;
    uint32_t v;
} stest;
static nu_result_t on_start(void)
{
    // const stest data[] = {
    //     {.d=10.0},
    //     {.d=11.0},
    //     {.d=12.0},
    //     {.d=13.0},
    //     {.d=15.0},
    // };
    // stest p = {
    //     .d = 66.0
    // };
    // nu_array_t ar;
    // nu_array_allocate_from(&ar, data, 5, sizeof(stest));
    // nu_array_push(&ar, &p);
    // nu_array_swap_last(ar, 2);
    // stest *last = (stest*)nu_array_get_last(ar);
    // nu_info("%lf\n", last->d);
    // stest *o = (stest*)nu_array_get(ar, 2);
    // nu_info("%lf\n", o->d);
    // nu_info("%d\n", nu_array_get_size(ar));
    // nu_info("%d\n", nu_array_get_capacity(ar));
    // nu_info("%d\n", nu_array_get_memory_consumption(ar));
    // nu_array_free(ar);

    // nu_string_t str0, str1;
    // nu_string_allocate_from(&str0, "Hello");
    // nu_string_allocate_from(&str1, "Bonjour");
    // nu_string_append(&str0, str1);
    // nu_info("%s\n", nu_string_get_cstr(str0));
    // nu_info("%d\n", nu_string_get_length(str0));
    // nu_string_free(str0);
    // nu_string_free(str1);

    // nu_string_t str0;
    // nu_string_allocate_from(&str0, "           ");
    // nu_string_trim(&str0);
    // nu_info("|%s|\n", nu_string_get_cstr(str0));
    // nu_info("%d\n", nu_string_get_length(str0));
    // nu_string_free(str0);

    // nu_string_t str0, str1;
    // nu_string_allocate_from(&str0, "123456");
    // nu_string_erase(&str0, 1, 3);
    // nu_info("%s\n", nu_string_get_cstr(str0));
    // nu_string_free(str0);

    // nu_array_t ar;
    // nu_array_allocate(&ar, sizeof(const char*));
    // const char *str = "Coucou!";
    // nu_array_push(&ar, &str);
    // const char *s = *(const char**)nu_array_get(ar, 0);
    // nu_info("%s\n", s);
    // nu_info("%d\n", nu_array_get_size(ar));
    // nu_array_free(ar);

    // nu_string_t str, token;
    // nu_string_tokens_t tokens;
    // nu_string_allocate_from(&str, "viewport   set 1029 1234");
    // nu_string_split(str, " ", &tokens);
    // uint32_t count = nu_string_tokens_get_length(tokens);
    // for (uint32_t i = 0; i < count; i++) {
    //     nu_string_allocate_from(&token, nu_string_tokens_get(tokens, i));
    //     // nu_string_trim(&token);
    //     nu_info("%d %s\n",
    //         nu_string_get_length(token), 
    //         nu_string_get_cstr(token));
    //     nu_string_free(token);
    // }
    // nu_string_tokens_free(tokens);
    // nu_string_free(str);

    // nu_indexed_array_t iar;
    // nu_array_t ids;
    // stest s;
    // nu_indexed_array_allocate(&iar, sizeof(uint32_t));
    // nu_array_allocate(&ids, sizeof(stest));

    // for (uint32_t i = 0; i < 10000; i++) {
    //     uint32_t len = nu_array_get_size(ids);
    //     if (rand() % 3 != 0) {
    //         /* add */
    //         uint32_t id;
    //         s.v  = rand() % 5000;
    //         nu_indexed_array_add(&iar, &s.v, &id);
    //         s.id = id;
    //         nu_array_push(&ids, &s);
    //     } else if (len > 0) {
    //         /* remove */
    //         nu_array_swap_last(ids, rand() % len);
    //         stest s = *(stest*)nu_array_get_last(ids);
    //         nu_array_pop(ids);

    //         /* test */
    //         uint32_t va = *(uint32_t*)nu_indexed_array_get(iar, s.id);
    //         NU_ASSERT(s.v == va);
    //         nu_indexed_array_remove(iar, s.id);
    //     }
    // }

    // nu_info("memory: %d\n", nu_indexed_array_get_memory_consumption(iar));

    // nu_indexed_array_free(iar);
    // nu_array_free(ids);

    nu_module_handle_t module;
    nu_module_load(&module, "engine/module/nucleus-utils");
    nu_plugin_require(module, NUUTILS_COMMAND_PLUGIN_NAME);
    nu_plugin_require(module, NUUTILS_CONSOLE_PLUGIN_NAME);
    nu_plugin_require(module, NUUTILS_SPECTATOR_PLUGIN_NAME);

    /* load texture */
    nu_renderer_texture_create_info_t texture_info = {0};
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
    nu_renderer_material_create_info_t material_info = {0};
    material_info.diffuse_texture      = rdr2_texture_id;
    material_info.normal_texture       = NU_NULL_HANDLE;
    material_info.specular_texture     = NU_NULL_HANDLE;
    material_info.specular_uniform     = 1.0f;

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
    nu_vec3f_t positions[] =
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
    nu_vec2f_t uvs[] =
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

    nu_renderer_submesh_info_t submesh_info = {0};
    submesh_info.indice_count     = indice_count;
    submesh_info.positions        = positions;
    submesh_info.uvs              = uvs;
    submesh_info.position_indices = position_indices;
    submesh_info.uv_indices       = uv_indices;
    nu_renderer_mesh_create_info_t mesh_info = {0};
    mesh_info.submeshes     = &submesh_info;
    mesh_info.submesh_count = 1;
    nu_renderer_mesh_handle_t mesh_id;
    if (nu_renderer_mesh_create(&mesh_id, &mesh_info) != NU_SUCCESS) {
        nu_warning("Failed to create cube mesh.\n");
    }

    /* create static meshes */
    nu_renderer_model_handle_t model_id;
    nu_renderer_model_create_info_t model_info = {0};
    model_info.mesh           = mesh_id;
    model_info.materials      = &material0;
    model_info.material_count = 1;

    nu_mat4f_identity(model_info.transform);
    
    nu_vec3f_t translation0 = {0.0f, -4.0f, 0.0f};
    nu_translate(model_info.transform, translation0);
    nu_vec3f_t scale0 = {100.0f, 0.1f, 100.0f};
    nu_scale(model_info.transform, scale0);
    nu_renderer_model_create(&model_id, &model_info);

    model_info.materials = &material1;

    for (uint32_t i = 0; i < 5; i++) {
        for (uint32_t j = 0; j < 5; j++) {
            for (uint32_t k = 0; k < 5; k++) {
                nu_mat4f_identity(model_info.transform);
                nu_vec3f_t translation1 = {i * 2.0f, k * 2.0f, j * 2.0f};
                nu_translate(model_info.transform, translation1);
                nu_vec3f_t scale1 = {0.5f, 0.5f, 0.5f};
                nu_scale(model_info.transform, scale1);
                nu_renderer_model_create(&model_id, &model_info);
            }
        }
    }

    /* create monkey */
    nu_renderer_mesh_handle_t mesh_handle;
    if (load_monkey(&mesh_handle) == NU_SUCCESS) {
        model_info.mesh           = mesh_handle;
        model_info.materials      = &alfred_material;
        model_info.material_count = 1;
        nu_mat4f_identity(model_info.transform);
        nu_vec3f_t translation2 = {0, 30, 0};
        nu_translate(model_info.transform, translation2);
        nu_vec3f_t scale2 = {4, 4, 4};
        nu_scale(model_info.transform, scale2);
        nu_renderer_model_create(&model_id, &model_info);
    } else {
        nu_warning("Failed to load monkey\n");
    }

    return NU_SUCCESS;
}
static nu_result_t on_stop(void)
{
    return NU_SUCCESS;
}
static nu_result_t on_update(void)
{
    /* quit handling */
    nu_button_state_t escape_state;
    nu_input_get_keyboard_state(&escape_state, NU_KEYBOARD_ESCAPE);
    if (escape_state & NU_BUTTON_PRESSED) {
        nu_context_request_stop();
    }

    return NU_SUCCESS;
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