#include <stdlib.h>
#include <math.h>

#include <nucleus/nucleus.h>
#include <nucleus/module/utils.h>
#include <nucleus/module/lua.h>
#include <nucleus/module/vulkan.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#define MAIN_LOGGER_NAME "MAIN"

static nu_result_t load_monkey(nu_renderer_mesh_t *mesh)
{
    nu_module_t module;
    nuutils_loader_interface_t loader;
    if (nu_module_get_by_name(NUUTILS_MODULE_NAME, &module) != NU_SUCCESS) return NU_FAILURE;
    if (nu_module_get_interface(module, NUUTILS_LOADER_INTERFACE_NAME, &loader) != NU_SUCCESS) return NU_FAILURE;
    if (loader.load_mesh_from_obj("$ENGINE_DIR/model/alfred/alfred.obj", mesh) != NU_SUCCESS) return NU_FAILURE;

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
        nu_info(MAIN_LOGGER_NAME, "delta %f", avg / (float)avg_count);
        avg = 0.0;
        avg_count = 0;
    }
}
typedef struct {
    uint32_t id;
    uint32_t v;
} stest;

static nuvk_sdf_interface_t SDF;

static nu_result_t parse_scene(void)
{
    nu_json_t json;
    nu_json_object_t root;
    nu_json_array_t instances;
    nu_result_t result;

    result = nu_json_allocate_from_file(&json, "$ENGINE_DIR/script/map.json");
    NU_CHECK(result == NU_SUCCESS, return NU_FAILURE, "MAIN", "Failed to parse json.");
    result = nu_json_value_as_object(nu_json_get_root(json), &root);
    NU_CHECK(result == NU_SUCCESS, goto cleanup0, "MAIN", "Failed to get root object.");
    result = nu_json_value_as_array(nu_json_object_get_by_name(root, "instances"), &instances);
    NU_CHECK(result == NU_SUCCESS, goto cleanup0, "MAIN", "Failed to get instances.");

    nu_json_array_iterator_t it = NU_NULL_HANDLE;
    while (nu_json_array_next(instances, &it)) {
        nu_json_object_t j_instance;
        result = nu_json_value_as_object(nu_json_array_iterator_get_value(it), &j_instance);
        NU_CHECK(result == NU_SUCCESS, goto cleanup0, "MAIN", "Instance is not a transform");

        const char *type;
        result = nu_json_value_as_cstr(nu_json_object_get_by_name(j_instance, "type"), &type);
        NU_CHECK(result == NU_SUCCESS, goto cleanup0, "MAIN", "Failed to get type.");

        nuvk_sdf_transform_t transform;
        nu_quatf_identity(transform.rotation);
        nu_json_value_t j_tranform = nu_json_object_get_by_name(j_instance, "transform");
        if (j_tranform != NU_NULL_HANDLE) {
            nu_json_object_t j_transform_object;
            result = nu_json_value_as_object(j_tranform, &j_transform_object);
            NU_CHECK(result == NU_SUCCESS, goto cleanup0, "MAIN", "Transform is not an object.");
            
            result = nu_json_value_as_vec3f(nu_json_object_get_by_name(j_transform_object, "translation"), transform.translation);
            NU_CHECK(result == NU_SUCCESS, goto cleanup0, "MAIN", "Failed to parse translation tranform.");

            result = nu_json_value_as_float(nu_json_object_get_by_name(j_transform_object, "scale"), &transform.scale);
            NU_CHECK(result == NU_SUCCESS, goto cleanup0, "MAIN", "Failed to parse scale tranform.");
        }

        nuvk_sdf_instance_t instance;
        nuvk_sdf_instance_info_t info;
        memset(&info, 0, sizeof(nuvk_sdf_instance_info_t));
        info.transform = transform;

        if (NU_MATCH(type, "sphere")) {
            nu_json_object_t j_data;
            result = nu_json_value_as_object(nu_json_object_get_by_name(j_instance, "data"), &j_data);
            NU_CHECK(result == NU_SUCCESS, goto cleanup0, "MAIN", "Failed to get data.");

            nuvk_sdf_sphere_data_t data;
            result = nu_json_value_as_float(nu_json_object_get_by_name(j_data, "radius"), &data.radius);
            NU_CHECK(result == NU_SUCCESS, goto cleanup0, "MAIN", "Failed to get radius.");

            nuvk_sdf_instance_type_t sphere_type;
            result = SDF.get_instance_type(NUVK_SDF_INSTANCE_TYPE_SPHERE, &sphere_type);
            NU_CHECK(result == NU_SUCCESS, goto cleanup0, "MAIN", "Failed to sphere type.");

            info.data = &data;
            info.type = sphere_type;
            result = SDF.create_instance(&info, &instance);
            NU_CHECK(result == NU_SUCCESS, goto cleanup0, "MAIN", "Failed to create sphere instance.");
        } else if (NU_MATCH(type, "cube")) {
            nu_json_object_t j_data;
            result = nu_json_value_as_object(nu_json_object_get_by_name(j_instance, "data"), &j_data);
            NU_CHECK(result == NU_SUCCESS, goto cleanup0, "MAIN", "Failed to get data.");

            nuvk_sdf_cube_data_t data;
            result = nu_json_value_as_float(nu_json_object_get_by_name(j_data, "radius"), &data.radius);
            NU_CHECK(result == NU_SUCCESS, goto cleanup0, "MAIN", "Failed to get radius.");
            result = nu_json_value_as_vec3f(nu_json_object_get_by_name(j_data, "box"), data.box);
            NU_CHECK(result == NU_SUCCESS, goto cleanup0, "MAIN", "Failed to get box.");

            nuvk_sdf_instance_type_t cube_type;
            result = SDF.get_instance_type(NUVK_SDF_INSTANCE_TYPE_CUBE, &cube_type);
            NU_CHECK(result == NU_SUCCESS, goto cleanup0, "MAIN", "Failed to cube type.");

            info.data = &data;
            info.type = cube_type;
            result = SDF.create_instance(&info, &instance);
            NU_CHECK(result == NU_SUCCESS, goto cleanup0, "MAIN", "Failed to create cube instance.");
        } else if (NU_MATCH(type, "torus")) {
            nu_json_object_t j_data;
            result = nu_json_value_as_object(nu_json_object_get_by_name(j_instance, "data"), &j_data);
            NU_CHECK(result == NU_SUCCESS, goto cleanup0, "MAIN", "Failed to get data.");

            nuvk_sdf_torus_data_t data;
            result = nu_json_value_as_float(nu_json_object_get_by_name(j_data, "x"), &data.x);
            NU_CHECK(result == NU_SUCCESS, goto cleanup0, "MAIN", "Failed to get x.");
            result = nu_json_value_as_float(nu_json_object_get_by_name(j_data, "y"), &data.y);
            NU_CHECK(result == NU_SUCCESS, goto cleanup0, "MAIN", "Failed to get y.");

            nuvk_sdf_instance_type_t torus_type;
            result = SDF.get_instance_type(NUVK_SDF_INSTANCE_TYPE_TORUS, &torus_type);
            NU_CHECK(result == NU_SUCCESS, goto cleanup0, "MAIN", "Failed to torus type.");

            info.data = &data;
            info.type = torus_type;
            result = SDF.create_instance(&info, &instance);
            NU_CHECK(result == NU_SUCCESS, goto cleanup0, "MAIN", "Failed to create torus instance.");
        } else if (NU_MATCH(type, "plane")) {
            nu_json_object_t j_data;
            result = nu_json_value_as_object(nu_json_object_get_by_name(j_instance, "data"), &j_data);
            NU_CHECK(result == NU_SUCCESS, goto cleanup0, "MAIN", "Failed to get data.");

            nuvk_sdf_plane_data_t data;
            result = nu_json_value_as_float(nu_json_object_get_by_name(j_data, "height"), &data.height);
            NU_CHECK(result == NU_SUCCESS, goto cleanup0, "MAIN", "Failed to get height.");
            result = nu_json_value_as_vec3f(nu_json_object_get_by_name(j_data, "normal"), data.normal);
            NU_CHECK(result == NU_SUCCESS, goto cleanup0, "MAIN", "Failed to get normal.");

            nuvk_sdf_instance_type_t plane_type;
            result = SDF.get_instance_type(NUVK_SDF_INSTANCE_TYPE_PLANE, &plane_type);
            NU_CHECK(result == NU_SUCCESS, goto cleanup0, "MAIN", "Failed to plane type.");

            info.data = &data;
            info.type = plane_type;
            result = SDF.create_instance(&info, &instance);
            NU_CHECK(result == NU_SUCCESS, goto cleanup0, "MAIN", "Failed to create plane instance.");
        }
    }

cleanup0:
    nu_json_free(json);

    return NU_SUCCESS;
}

static nu_result_t on_start(void)
{
    // nu_string_t token;
    // nu_string_array_t tokens;
    // nu_string_array_allocate(&tokens);
    // nu_string_split_cstr("set viewport\n 123 321\n", "\n", tokens);
    // uint32_t count = nu_string_array_get_length(tokens);
    // for (uint32_t i = 0; i < count; i++) {
    //     nu_string_allocate_cstr(nu_string_array_get(tokens, i), &token);
    //     nu_info("main", "%ld %s",
    //         nu_string_get_length(token), 
    //         nu_string_get_cstr(token));
    //     nu_string_free(token);
    // }
    // nu_string_array_free(tokens);

    // nu_interrupt("main", "lol");

    // nu_timer_t t;
    // nu_timer_allocate(&t);
    
    // nu_array_t ar;
    // uint32_t v = 555;
    // nu_array_allocate_capacity(&ar, sizeof(uint32_t), 50000000);
    // nu_timer_start(&t);
    // for (uint32_t i = 50000000; i--;) {
    //     nu_array_push(ar, &v);
    // }
    // float d = nu_timer_get_time_elapsed(&t);
    // printf("%lf\n", d);
    // nu_array_free(ar);

    // std::vector<uint32_t> vec;
    // nu_timer_start(&t);
    // for (uint32_t i = 50000000; i--;) {
    //     vec.emplace_back(v);
    // }
    // d = nu_timer_get_time_elapsed(&t);
    // printf("%lf\n", d);

    // nu_array_t ar;
    // stest v0, v1;
    // v0.v = 6;
    // v1.v = 2;
    // nu_array_allocate_capacity(&ar, sizeof(stest), 5);
    // nu_array_push(ar, &v0);
    // nu_array_push(ar, &v1);
    // nu_array_swap_last(ar, 0);
    // stest p = *(stest*)nu_array_get_last(ar);
    // nu_info("%d\n", p.v);
    // nu_array_free(ar);
    // nu_timer_free(t);


    // nu_indexed_array_t ar;
    // stest v;
    // uint32_t id;
    // nu_indexed_array_allocate(sizeof(stest), &ar);
    // nu_indexed_array_add(ar, &v, &id);
    // nu_info("index capacity %d\n", nu_indexed_array_get_index_capacity(ar));
    // nu_info("capacity       %d\n", nu_indexed_array_get_capacity(ar));
    // nu_info("memory         %d\n", nu_indexed_array_get_allocated_memory(ar));
    // nu_indexed_array_free(ar);

    // nu_indexed_array_t iar;
    // nu_array_t ids;
    // stest s;
    // nu_indexed_array_allocate(sizeof(uint32_t), &iar);
    // nu_array_allocate(&ids, sizeof(stest));

    // for (uint32_t i = 0; i < 10000000; i++) {
    //     uint32_t len = nu_array_get_size(ids);
    //     if (rand() % 3 != 0) {
    //         /* add */
    //         uint32_t id;
    //         s.v  = rand() % 5000;
    //         nu_indexed_array_add(iar, &s.v, &id);
    //         s.id = id;
    //         nu_array_push(ids, &s);
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

    // nu_info("memory: %d\n", nu_indexed_array_get_allocated_memory(iar));
    // nu_info("size:   %d\n", nu_indexed_array_get_size(iar));
    // nu_info("index:  %d\n", nu_indexed_array_get_index_capacity(iar));
    // nu_info("data:   %d\n", nu_indexed_array_get_capacity(iar));

    // nu_indexed_array_free(iar);
    // nu_array_free(ids);


    nu_module_t module;
    NU_ASSERT(nu_module_load("$MODULE_DIR/nucleus-utils", &module) == NU_SUCCESS);
    nu_plugin_require(module, NUUTILS_COMMAND_PLUGIN_NAME);
    nu_plugin_require(module, NUUTILS_CONSOLE_PLUGIN_NAME);
    // nu_plugin_require(module, NUUTILS_SPECTATOR_PLUGIN_NAME);

    nu_module_t lua_module;
    NU_ASSERT(nu_module_load("$MODULE_DIR/nucleus-lua", &lua_module) == NU_SUCCESS);
    nu_plugin_require(lua_module, NULUA_PLUGIN_NAME);

    /* load sdf interface */
    nu_module_t renderer_module = nu_renderer_get_module();
    nu_module_get_interface(renderer_module, NUVK_SDF_INTERFACE_NAME, &SDF);

    /* load lua plugin */
    nulua_plugin_interface_t lua_interface;
    NU_ASSERT(nu_module_get_interface(lua_module, NULUA_PLUGIN_INTERFACE_NAME, &lua_interface) == NU_SUCCESS);
    nulua_plugin_t plugin;
    NU_ASSERT(lua_interface.load_plugin("$ENGINE_DIR/script/test.lua", &plugin));
    NU_ASSERT(lua_interface.load_plugin("$ENGINE_DIR/script/spectator.lua", &plugin));

    /* load texture */
    nu_renderer_texture_create_info_t texture_info = {0};
    int width, height, channel;
    unsigned char *ima_data;

    ima_data = stbi_load("engine/texture/brick.jpg", &width, &height, &channel, STBI_rgb);
    if (!ima_data) nu_interrupt(MAIN_LOGGER_NAME, "Failed to load brick texture.\n");
    texture_info.width   = (uint32_t)width;
    texture_info.height  = (uint32_t)height;
    texture_info.channel = (uint32_t)channel;
    texture_info.data    = ima_data;
    nu_renderer_texture_t brick_texture_id;
    if (nu_renderer_texture_create(&texture_info, &brick_texture_id) != NU_SUCCESS) {
        nu_warning(MAIN_LOGGER_NAME, "Failed to create texture.");
    }
    stbi_image_free(ima_data);

    ima_data = stbi_load("engine/texture/checkerboard.jpg", &width, &height, &channel, STBI_rgb);
    if (!ima_data) nu_interrupt(MAIN_LOGGER_NAME, "Failed to load rdr2 texture.\n");
    texture_info.width   = (uint32_t)width;
    texture_info.height  = (uint32_t)height;
    texture_info.channel = (uint32_t)channel;
    texture_info.data    = ima_data;
    nu_renderer_texture_t rdr2_texture_id;
    if (nu_renderer_texture_create(&texture_info, &rdr2_texture_id) != NU_SUCCESS) {
        nu_warning(MAIN_LOGGER_NAME, "Failed to create texture.");
    }
    stbi_image_free(ima_data);

    ima_data = stbi_load("engine/model/alfred/alfred.jpg", &width, &height, &channel, STBI_rgb);
    if (!ima_data) nu_interrupt(MAIN_LOGGER_NAME, "Failed to load alfred texture.\n");
    texture_info.width   = (uint32_t)width;
    texture_info.height  = (uint32_t)height;
    texture_info.channel = (uint32_t)channel;
    texture_info.data = ima_data;
    nu_renderer_texture_t alfred_texture;
    if (nu_renderer_texture_create(&texture_info, &alfred_texture) != NU_SUCCESS) {
        nu_warning(MAIN_LOGGER_NAME, "Failed to create texture.");
    }
    stbi_image_free(ima_data);

    /* create materials */
    nu_renderer_material_create_info_t material_info = {0};
    material_info.diffuse_texture  = rdr2_texture_id;
    material_info.normal_texture   = NU_NULL_HANDLE;
    material_info.specular_texture = NU_NULL_HANDLE;
    material_info.specular_uniform = 1.0f;

    nu_renderer_material_t material0 = {0};
    if (nu_renderer_material_create(&material_info, &material0) != NU_SUCCESS) {
        nu_interrupt(MAIN_LOGGER_NAME, "Failed to create material0.");
    }

    nu_renderer_material_t material1 = {0};
    material_info.diffuse_texture = brick_texture_id;
    if (nu_renderer_material_create(&material_info, &material1) != NU_SUCCESS) {
        nu_interrupt(MAIN_LOGGER_NAME, "Failed to create material1.");
    }

    nu_renderer_material_t alfred_material = {0};
    material_info.diffuse_texture = alfred_texture;
    if (nu_renderer_material_create(&material_info, &alfred_material) != NU_SUCCESS) {
        nu_interrupt(MAIN_LOGGER_NAME, "Failed to create alfred_material.");
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
    nu_renderer_mesh_t mesh_id;
    if (nu_renderer_mesh_create(&mesh_info, &mesh_id) != NU_SUCCESS) {
        nu_warning(MAIN_LOGGER_NAME, "Failed to create cube mesh.");
    }

    /* create static meshes */
    nu_renderer_model_t model_id;
    nu_renderer_model_create_info_t model_info = {0};
    model_info.mesh           = mesh_id;
    model_info.materials      = &material0;
    model_info.material_count = 1;

    nu_mat4f_identity(model_info.transform);
    
    nu_vec3f_t translation0 = {0.0f, -4.0f, 0.0f};
    nu_mat4f_translate(model_info.transform, translation0);
    nu_vec3f_t scale0 = {100.0f, 0.1f, 100.0f};
    nu_mat4f_scale(model_info.transform, scale0);
    nu_renderer_model_create(&model_info, &model_id);

    model_info.materials = &material1;

    for (uint32_t i = 0; i < 5; i++) {
        for (uint32_t j = 0; j < 5; j++) {
            for (uint32_t k = 0; k < 5; k++) {
                nu_mat4f_identity(model_info.transform);
                nu_vec3f_t translation1 = {i * 2.0f, k * 2.0f, j * 2.0f};
                nu_mat4f_translate(model_info.transform, translation1);
                nu_vec3f_t scale1 = {0.5f, 0.5f, 0.5f};
                nu_mat4f_scale(model_info.transform, scale1);
                nu_renderer_model_create(&model_info, &model_id);
            }
        }
    }

    /* create monkey */
    nu_renderer_mesh_t mesh_handle;
    if (load_monkey(&mesh_handle) == NU_SUCCESS) {
        model_info.mesh           = mesh_handle;
        model_info.materials      = &alfred_material;
        model_info.material_count = 1;
        nu_mat4f_identity(model_info.transform);
        nu_vec3f_t translation2 = {0, 30, 0};
        nu_mat4f_translate(model_info.transform, translation2);
        nu_vec3f_t scale2 = {4, 4, 4};
        nu_mat4f_scale(model_info.transform, scale2);
        nu_renderer_model_create(&model_info, &model_id);
    } else {
        nu_warning(MAIN_LOGGER_NAME, "Failed to load monkey");
    }

    return NU_SUCCESS;
}
static nu_result_t on_stop(void)
{
    return NU_SUCCESS;
}
static nu_result_t on_update(void)
{
    static int once = 1;
    if (once) {

        nu_result_t result = parse_scene();
        if (result != NU_SUCCESS) {
            nu_interrupt("MAIN", "Failed to parse scene.");
        }   

        once = 0;
    }

    /* quit handling */
    nu_button_state_t escape_state;
    nu_input_get_keyboard_state(NU_KEYBOARD_ESCAPE, &escape_state);
    if (escape_state & NU_BUTTON_PRESSED) {
        nu_context_request_stop();
    }

    nu_button_state_t f6_state, f7_state, f8_state;
    nu_input_get_keyboard_state(NU_KEYBOARD_F6, &f6_state);
    nu_input_get_keyboard_state(NU_KEYBOARD_F7, &f7_state);
    nu_input_get_keyboard_state(NU_KEYBOARD_F8, &f8_state);
    if (f6_state & NU_BUTTON_JUST_PRESSED) {
        nu_window_set_mode(NU_WINDOW_MODE_WINDOWED);
    } else if (f7_state & NU_BUTTON_JUST_PRESSED) {
        nu_window_set_mode(NU_WINDOW_MODE_BORDERLESS);
    } else if (f8_state & NU_BUTTON_JUST_PRESSED) {
        nu_window_set_mode(NU_WINDOW_MODE_FULLSCREEN);
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