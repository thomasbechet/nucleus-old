#include <stdlib.h>
#include <math.h>

#include <nucleus/nucleus.h>
#include <nucleus/module/utils.h>
#include <nucleus/module/lua.h>
#include <nucleus/module/vulkan.h>
#include <nucleus/module/ecs.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#define MAIN_LOGGER_NAME "MAIN"

static nuvk_renderer_interface_t renderer;

static nu_result_t parse_scene(void)
{
    nu_result_t result;

    nuvk_material_info_t material_info;
    memset(&material_info, 0, sizeof(nuvk_material_info_t));
    nu_vec3f_copy((nu_vec3f_t){1, 1, 1}, material_info.color);

    nuvk_material_t material0;
    result = renderer.material_create(&material_info, &material0);
    NU_CHECK(result == NU_SUCCESS, return NU_FAILURE, "MAIN", "Failed to create material 0.");

    nu_json_t json;
    nu_json_object_t root;
    nu_json_array_t instances;

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

            nuvk_sdf_t sphere_sdf;
            result = renderer.sdf_get_primitive(NUVK_SDF_PRIMITIVE_SPHERE, &sphere_sdf);
            NU_CHECK(result == NU_SUCCESS, goto cleanup0, "MAIN", "Failed to sphere primitive.");

            info.data     = &data;
            info.sdf      = sphere_sdf;
            info.material = material0;
            result = renderer.sdf_instance_create(&info, &instance);
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
            result = renderer.sdf_get_primitive(NUVK_SDF_PRIMITIVE_CUBE, &cube_sdf);
            NU_CHECK(result == NU_SUCCESS, goto cleanup0, "MAIN", "Failed to cube primitive.");

            info.data     = &data;
            info.sdf      = cube_sdf;
            info.material = material0;
            result = renderer.sdf_instance_create(&info, &instance);
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
            result = renderer.sdf_get_primitive(NUVK_SDF_PRIMITIVE_TORUS, &torus_sdf);
            NU_CHECK(result == NU_SUCCESS, goto cleanup0, "MAIN", "Failed to torus primitive.");

            info.data     = &data;
            info.sdf      = torus_sdf;
            info.material = material0;
            result = renderer.sdf_instance_create(&info, &instance);
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
            result = renderer.sdf_get_primitive(NUVK_SDF_PRIMITIVE_PLANE, &plane_sdf);
            NU_CHECK(result == NU_SUCCESS, goto cleanup0, "MAIN", "Failed to plane primitive.");

            info.data     = &data;
            info.sdf      = plane_sdf;
            info.material = material0;
            result = renderer.sdf_instance_create(&info, &instance);
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
static uint32_t nu_random()
{
    static uint32_t next = 1231125;
    next = (unsigned int)(next * 123423542) % 23423425;
    return next;
}
static nu_result_t nuecs_system0_update(nuecs_component_data_ptr_t *components, uint32_t count)
{
    // nu_info("test", "update %d", count);
    // position_t *positions = (position_t*)components[0];
    // health_t   *healthes  = (health_t*)components[1];
    // for (uint32_t i = 0; i < count; i++) {
    //     nu_info("test", "health %lf", healthes[i].value);
    // }

    return NU_SUCCESS;
}
static nu_result_t nuecs_system2_update(nuecs_component_data_ptr_t *components, uint32_t count)
{
    // nu_info("system2", "%d", count);
    return NU_SUCCESS;
}
static nu_result_t on_start(void)
{
    nu_module_t module;
    NU_ASSERT(nu_module_load("$MODULE_DIR/nucleus-utils", &module) == NU_SUCCESS);
    nu_plugin_require(module, NUUTILS_COMMAND_PLUGIN_NAME);

    nu_module_t lua_module;
    NU_ASSERT(nu_module_load("$MODULE_DIR/nucleus-lua", &lua_module) == NU_SUCCESS);
    nu_plugin_require(lua_module, NULUA_PLUGIN_NAME);

    nu_module_t ecs_module;
    NU_ASSERT(nu_module_load("$MODULE_DIR/nucleus-ecs", &ecs_module) == NU_SUCCESS);
    nu_plugin_require(ecs_module, NUECS_PLUGIN_NAME);

    /* load sdf interface */
    nu_module_t renderer_module = nu_renderer_get_module();
    nu_module_get_interface(renderer_module, NUVK_RENDERER_INTERFACE_NAME, &renderer);

    /* load lua interface */
    nulua_plugin_interface_t lua_interface;
    NU_ASSERT(nu_module_get_interface(lua_module, NULUA_PLUGIN_INTERFACE_NAME, &lua_interface) == NU_SUCCESS);
    nulua_plugin_t plugin;
    NU_ASSERT(lua_interface.load_plugin("$ENGINE_DIR/script/test.lua", &plugin));
    NU_ASSERT(lua_interface.load_plugin("$ENGINE_DIR/script/spectator.lua", &plugin));

    /* load ecs interface */
    nuecs_plugin_interface_t ecs_interface;
    NU_ASSERT(nu_module_get_interface(ecs_module, NUECS_PLUGIN_INTERFACE_NAME, &ecs_interface) == NU_SUCCESS);

    nuecs_world_t world;
    NU_ASSERT(ecs_interface.world_create(&world) == NU_SUCCESS);

    nuecs_component_t position_component, health_component, velocity_component, score_component;
    NUECS_REGISTER_COMPONENT(ecs_interface, world, position_t, position_component);
    NUECS_REGISTER_COMPONENT(ecs_interface, world, health_t, health_component);
    NUECS_REGISTER_COMPONENT(ecs_interface, world, velocity_t, velocity_component);
    NUECS_REGISTER_COMPONENT(ecs_interface, world, score_t, score_component);

    position_t position;
    nu_vec3f_copy((nu_vec3f_t){0, 1, 2}, position.pos);
    health_t health;
    velocity_t velocity;
    score_t score;

    nuecs_component_t components0[] = {position_component, health_component, velocity_component};
    nuecs_component_t components1[] = {position_component, health_component};
    nuecs_component_t components2[] = {position_component, score_component};
    nuecs_component_data_ptr_t data0[] = {&position, &health, &velocity};
    nuecs_component_data_ptr_t data1[] = {&position, &health};
    nuecs_component_data_ptr_t data2[] = {&position, &score};
    nuecs_entity_t entity0, entity1, entity2;
    health.value = 0.0f;
    NUECS_CREATE_ENTITY(ecs_interface, world, components0, data0, entity0);
    health.value = 1.0f;
    NUECS_CREATE_ENTITY(ecs_interface, world, components1, data1, entity1);
    health.value = 2.0f;
    NUECS_CREATE_ENTITY(ecs_interface, world, components1, data1, entity1);
    NUECS_CREATE_ENTITY(ecs_interface, world, components2, data2, entity2);
    for (uint32_t i = 0; i < 10000; i++) {
        NUECS_CREATE_ENTITY(ecs_interface, world, components2, data2, entity2);
    }

    nuecs_system_t system0, system2;
    NUECS_REGISTER_SYSTEM(ecs_interface, world, components1, nuecs_system0_update, system0);
    NUECS_REGISTER_SYSTEM(ecs_interface, world, components2, nuecs_system2_update, system2);

    // nu_info("WORLD", "START");
    // ecs_interface.world_progress(world);
    // ecs_interface.entity_destroy(world, entity0);
    // nu_info("WORLD", "REMOVE");
    // ecs_interface.world_progress(world);
    // nu_info("WORLD", "ADD");
    // NUECS_CREATE_ENTITY(ecs_interface, world, components0, data0, entity0);
    // ecs_interface.world_progress(world);
    // nu_info("WORLD", "END");

    /* load texture */
    int width, height, channel;
    unsigned char *ima_data;
    ima_data = stbi_load("engine/texture/brick.jpg", &width, &height, &channel, STBI_rgb);
    if (!ima_data) nu_interrupt(MAIN_LOGGER_NAME, "Failed to load brick texture.\n");
    stbi_image_free(ima_data);

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