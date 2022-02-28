#include <stdlib.h>
#include <math.h>

#include <nucleus/nucleus.h>
#include <nucleus/module/utils.h>
#define NULUA_LOADER_IMPLEMENTATION
#include <nucleus/module/lua.h>
#define NUVK_LOADER_IMPLEMENTATION
#include <nucleus/module/vulkan.h>
#define NUECS_LOADER_IMPLEMENTATION
#include <nucleus/module/ecs.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#define MAIN_LOGGER_NAME "MAIN"

static nu_result_t parse_scene(void)
{
    nu_result_t result;

    nuvk_material_info_t material_info;
    memset(&material_info, 0, sizeof(nuvk_material_info_t));
    nu_vec3f_copy((nu_vec3f_t){1, 1, 1}, material_info.color);

    nuvk_material_t material0;
    result = nuvk_renderer_material_create(&material_info, &material0);
    NU_CHECK(result == NU_SUCCESS, return NU_FAILURE, "MAIN", "Failed to create material 0.");

    nu_json_t json;
    nu_json_object_t root;
    nu_json_array_t instances;

    result = nu_json_allocate_from_file(&json, "$ENGINE/script/map.json");
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

            nuvk_sdf_t sphere_sdf;
            result = nuvk_renderer_sdf_get_primitive(NUVK_SDF_PRIMITIVE_SPHERE, &sphere_sdf);
            NU_CHECK(result == NU_SUCCESS, goto cleanup0, "MAIN", "Failed to sphere primitive.");

            info.data     = &data;
            info.sdf      = sphere_sdf;
            info.material = material0;
            result = nuvk_renderer_sdf_instance_create(&info, &instance);
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

            nuvk_sdf_t cube_sdf;
            result = nuvk_renderer_sdf_get_primitive(NUVK_SDF_PRIMITIVE_CUBE, &cube_sdf);
            NU_CHECK(result == NU_SUCCESS, goto cleanup0, "MAIN", "Failed to cube primitive.");

            info.data     = &data;
            info.sdf      = cube_sdf;
            info.material = material0;
            result = nuvk_renderer_sdf_instance_create(&info, &instance);
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

            nuvk_sdf_t torus_sdf;
            result = nuvk_renderer_sdf_get_primitive(NUVK_SDF_PRIMITIVE_TORUS, &torus_sdf);
            NU_CHECK(result == NU_SUCCESS, goto cleanup0, "MAIN", "Failed to torus primitive.");

            info.data     = &data;
            info.sdf      = torus_sdf;
            info.material = material0;
            result = nuvk_renderer_sdf_instance_create(&info, &instance);
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

            nuvk_sdf_t plane_sdf;
            result = nuvk_renderer_sdf_get_primitive(NUVK_SDF_PRIMITIVE_PLANE, &plane_sdf);
            NU_CHECK(result == NU_SUCCESS, goto cleanup0, "MAIN", "Failed to plane primitive.");

            info.data     = &data;
            info.sdf      = plane_sdf;
            info.material = material0;
            result = nuvk_renderer_sdf_instance_create(&info, &instance);
            NU_CHECK(result == NU_SUCCESS, goto cleanup0, "MAIN", "Failed to create plane instance.");
        }
    }

cleanup0:
    nu_json_free(json);

    return NU_SUCCESS;
}

typedef struct {
    nu_vec3f_t pos;
} position_t;
typedef struct {
    float value;
} health_t;
typedef struct {
    nu_vec3f_t velocity;
} velocity_t;
typedef struct {
    uint32_t score;
} score_t;

typedef struct {
    uint32_t id;
    uint32_t v;
} stest;

static nu_result_t serialize_position(const nuecs_component_data_ptr_t data, nu_json_object_t json)
{
    const position_t *position = (const position_t*)data;
    nu_json_object_put_vec3f(json, "value", position->pos);
    return NU_SUCCESS;
}
static nu_result_t serialize_health(const nuecs_component_data_ptr_t data, nu_json_object_t json)
{
    const health_t *health = (const health_t*)data;
    nu_json_object_put_float(json, "value", health->value);
    return NU_SUCCESS;
}
static nu_result_t serialize_velocity(const nuecs_component_data_ptr_t data, nu_json_object_t json)
{
    const velocity_t *velocity = (const velocity_t*)data;
    nu_json_object_put_vec3f(json, "value", velocity->velocity);
    return NU_SUCCESS;  
}
static nu_result_t serialize_score(const nuecs_component_data_ptr_t data, nu_json_object_t json)
{
    const score_t *score = (const score_t*)data;
    nu_json_object_put_float(json, "value", score->score);
    return NU_SUCCESS;
}

static uint32_t nu_random()
{
    static uint32_t next = 1231125;
    next = (unsigned int)(next * 123423542) % 23423425;
    return next;
}
static nu_result_t nuecs_system0_update(nuecs_component_data_ptr_t *components, uint32_t count)
{
    nu_info("test", "update [health, component] 0 %d", count);
    health_t   *healthes  = (health_t*)components[0];
    position_t *positions = (position_t*)components[1];
    for (uint32_t i = 0; i < count; i++) {
        nu_info("test", "position %lf health %lf", positions[i].pos[0], healthes[i].value);
    }
    return NU_SUCCESS;
}
static nu_result_t nuecs_system1_update(nuecs_component_data_ptr_t *components, uint32_t count)
{
    nu_info("test", "update [position] 1 %d", count);
    position_t *positions = (position_t*)components[0];
    for (uint32_t i = 0; i < count; i++) {
        nu_info("test", "position %lf", positions[i].pos[0]);
    }
    return NU_SUCCESS;
}
static nu_result_t nuecs_system2_update(nuecs_component_data_ptr_t *components, uint32_t count)
{
    nu_info("test", "update [position, velocity] 2 %d", count);
    velocity_t *velocities = (velocity_t*)components[1];
    for (uint32_t i = 0; i < count; i++) {
        nu_info("test", "velocity %lf", velocities[i].velocity[0]);
    }
    return NU_SUCCESS;
}
static nu_result_t on_start(void)
{
    nu_module_t module;
    NU_ASSERT(nu_module_load("$MODULE/nucleus-utils", &module) == NU_SUCCESS);
    nu_plugin_require(module, NUUTILS_COMMAND_PLUGIN_NAME);

    nu_module_t lua_module;
    NU_ASSERT(nu_module_load("$MODULE/nucleus-lua", &lua_module) == NU_SUCCESS);
    nu_plugin_require(lua_module, NULUA_MANAGER_PLUGIN_NAME);

    nu_module_t ecs_module;
    NU_ASSERT(nu_module_load("$MODULE/nucleus-ecs", &ecs_module) == NU_SUCCESS);
    nu_plugin_require(ecs_module, NUECS_SCENE_PLUGIN_NAME);
    nuecs_scene_interface_load(ecs_module);
    nuecs_manager_interface_load(ecs_module);

    /* load sdf interface */
    nu_module_t renderer_module = nu_renderer_get_module();
    nuvk_renderer_interface_load(renderer_module);

    /* load lua interface */
    NU_ASSERT(nulua_manager_interface_load(lua_module) == NU_SUCCESS);
    nulua_plugin_t plugin;
    NU_ASSERT(nulua_manager_load_plugin("$ENGINE/script/test.lua", &plugin));
    NU_ASSERT(nulua_manager_load_plugin("$ENGINE/script/spectator.lua", &plugin));

    /* load ecs interface */
    // nuecs_scene_t scene;
    // NU_ASSERT(nuecs_scene_create(&scene) == NU_SUCCESS);

    // nuecs_component_t position_component, health_component, velocity_component, score_component;
    // {
    //     nuecs_component_info_t info;
    //     info.name             = "position_t";
    //     info.size             = sizeof(position_t);
    //     info.serialize_json   = serialize_position;
    //     info.deserialize_json = NULL;
    //     nuecs_scene_register_component(scene, &info, &position_component);
    // }
    // {
    //     nuecs_component_info_t info;
    //     info.name             = "health_t";
    //     info.size             = sizeof(health_t);
    //     info.serialize_json   = serialize_health;
    //     info.deserialize_json = NULL;
    //     nuecs_scene_register_component(scene, &info, &health_component);
    // }
    // {
    //     nuecs_component_info_t info;
    //     info.name             = "velocity_t";
    //     info.size             = sizeof(velocity_t);
    //     info.serialize_json   = serialize_velocity;
    //     info.deserialize_json = NULL;
    //     nuecs_scene_register_component(scene, &info, &velocity_component);
    // }
    // {
    //     nuecs_component_info_t info;
    //     info.name             = "score_t";
    //     info.size             = sizeof(score_t);
    //     info.serialize_json   = serialize_score;
    //     info.deserialize_json = NULL;
    //     nuecs_scene_register_component(scene, &info, &score_component);
    // }

    position_t position;
    nu_vec3f_copy((nu_vec3f_t){3, 1, 2}, position.pos);
    health_t health = {2.0f};
    velocity_t velocity;
    nu_vec3f_copy((nu_vec3f_t){5, 1, 2}, velocity.velocity);
    score_t score;
    
    // nuecs_system_t system0, system1, system2;
    // nuecs_component_t system0_components[] = {health_component, position_component};
    // NUECS_REGISTER_SYSTEM(scene, system0_components, nuecs_system0_update, system0);
    // nuecs_component_t system1_components[] = {position_component};
    // NUECS_REGISTER_SYSTEM(scene, system1_components, nuecs_system1_update, system1);
    // nuecs_component_t system2_components[] = {position_component, velocity_component};
    // NUECS_REGISTER_SYSTEM(scene, system2_components, nuecs_system2_update, system2);

    // for (uint32_t i = 0; i < 100; i++) {
    //     nuecs_entity_t entity0;
    //     nuecs_entity_info_t info1;
    //     info1.components      = (nuecs_component_t[]){position_component, velocity_component};
    //     info1.component_data  = (nuecs_component_data_ptr_t[]){&position, &velocity};
    //     info1.component_count = 2;
    //     nuecs_scene_create_entity(scene, &info1, &entity0);
    // }

    // nu_info("WORLD", "start");
    // nuecs_scene_progress(scene);

    // nu_info("WORLD", "add velocity component");
    // ecs.entity_add_component(scene, entity0, velocity_component, &velocity);
    // ecs.scene_progress(scene);

    // nu_info("WORLD", "remove velocity component");
    // nuecs_scene_entity_remove_component(scene, entity0, velocity_component);
    // nuecs_scene_progress(scene);

    // nu_info("WORLD", "remove position component");
    // nuecs_scene_entity_remove_component(scene, entity0, position_component);
    // nuecs_scene_progress(scene);

    // nu_info("WORLD", "destroy");
    // ecs.entity_destroy(scene, entity0);
    // ecs.scene_progress(scene);
    
    // nu_info("WORLD", "end");

    // nuecs_scene_save_file(scene, "$ROOT/mywork.json");
    // nu_context_request_stop();

    /* load texture */
    int width, height, channel;
    unsigned char *ima_data;
    ima_data = stbi_load("engine/texture/brick.jpg", &width, &height, &channel, STBI_rgb);
    if (!ima_data) nu_interrupt(MAIN_LOGGER_NAME, "Failed to load brick texture.\n");
    stbi_image_free(ima_data);

    /* test json */
    {
        nu_json_t j;
        nu_json_object_t o;
        nu_json_allocate_empty_object(&j);
        nu_json_value_as_object(nu_json_get_root(j), &o);
        nu_json_object_put_cstr(o, "coucou", "salut !");
        nu_json_object_put_double(o, "test", 123.45252);
        nu_json_array_t a;
        nu_json_object_put_empty_array(o, "array", &a);
        nu_json_object_t sub;
        nu_json_object_put_empty_object(o, "object", &sub);
        nu_json_object_put_bool(o, "boolean", false);
        nu_vec3f_t v = {0, 1, 2};
        nu_json_object_put_vec3f(o, "myvector", v);

        nu_transform_t t;
        nu_transform_identity(&t);
        nu_json_object_put_transform(o, "transform",  &t);

        nu_json_array_t array;
        nu_json_object_put_empty_array(o, "mylist", &array);
        nu_json_array_add_transform(array, &t);
        nu_json_array_add_transform(array, &t);
        nu_json_array_add_transform(array, &t);
        nu_json_array_add_uint(array, 12345);
        nu_json_array_add_cstr(array, "coucou ma biche !");

        nu_json_save_file(j, "$ROOT/test.json", true);
        nu_json_free(j);
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