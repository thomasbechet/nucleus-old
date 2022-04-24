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
    nuecs_entity_reference_t next;
    nuecs_entity_reference_t prev;
    nuecs_entity_reference_t parent;
    nuecs_entity_reference_t child;
    bool dirty;
} transform_t;

// static nu_result_t serialize_transform(const nuecs_component_data_ptr_t data, nu_json_object_t json)
// {
//     const transform_t *transform = (const transform_t*)data;
//     // nuecs_serialize_entity(transform->child, "child", json);
//     nuecs_entity_serialize()
//     return NU_SUCCESS;
// }

typedef struct {
    nuecs_query_t player_query;
} move_player_system_t;

static nu_result_t move_player_start(void *state, nuecs_scene_t scene)
{
    move_player_system_t *system = (move_player_system_t*)state;
    nuecs_component_t player_component;
    nuecs_component_find("player", &player_component);
    nuecs_query_info_t info = {
        .exclude_component_count = 0,
        .include_component_count = 1,
        .include_components      = &player_component
    };
    nuecs_query_create(scene, &info, &system->player_query);
    nu_info("test", "START");
    return NU_SUCCESS;
}
static nu_result_t move_player_stop(void *state, nuecs_scene_t scene)
{
    nu_info("test", "STOP");
    move_player_system_t *system = (move_player_system_t*)state;
    nuecs_query_destroy(scene, system->player_query);
    return NU_SUCCESS;
}
static nu_result_t move_player_update(void *state, nuecs_scene_t scene)
{
    move_player_system_t *system = (move_player_system_t*)state;
    nu_info("test", "player system !");
    return NU_SUCCESS;
}

typedef struct {
    uint32_t id;
    uint32_t v;
} stest;

static nu_result_t serialize_position(nuecs_component_data_ptr_t data, nuecs_serialization_context_t ctx, nu_json_object_t json)
{
    position_t *position = (position_t*)data;
    nu_json_object_put_vec3f(json, "value", position->pos);
    return NU_SUCCESS;
}
static nu_result_t deserialize_position(nuecs_component_data_ptr_t data, nuecs_deserialization_context_t ctx, nu_json_object_t json)
{
    position_t *position = (position_t*)data;
    nu_json_value_as_vec3f(nu_json_object_get_by_name(json, "value"), position->pos);
    return NU_SUCCESS;
}
static nu_result_t serialize_health(nuecs_component_data_ptr_t data, nuecs_serialization_context_t ctx, nu_json_object_t json)
{
    health_t *health = (health_t*)data;
    nu_json_object_put_float(json, "value", health->value);
    return NU_SUCCESS;
}
static nu_result_t serialize_velocity(nuecs_component_data_ptr_t data, nuecs_serialization_context_t ctx, nu_json_object_t json)
{
    velocity_t *velocity = (velocity_t*)data;
    nu_json_object_put_vec3f(json, "value", velocity->velocity);
    return NU_SUCCESS;
}
static nu_result_t serialize_score(nuecs_component_data_ptr_t data, nuecs_serialization_context_t ctx, nu_json_object_t json)
{
    score_t *score = (score_t*)data;
    nu_json_object_put_float(json, "value", score->score);
    return NU_SUCCESS;
}
static nu_result_t serialize_transform(nuecs_component_data_ptr_t data, nuecs_serialization_context_t ctx, nu_json_object_t json)
{
    transform_t *transform = (transform_t*)data;
    nuecs_entity_reference_serialize_json_object(&transform->child, ctx, json, "child");
    nuecs_entity_reference_serialize_json_object(&transform->next, ctx, json, "next");
    nuecs_entity_reference_serialize_json_object(&transform->parent, ctx, json, "parent");
    nuecs_entity_reference_serialize_json_object(&transform->prev, ctx, json, "prev");
    nu_json_object_put_bool(json, "dirty", transform->dirty);
    return NU_SUCCESS;
}
static nu_result_t deserialize_transform(nuecs_component_data_ptr_t data, nuecs_deserialization_context_t ctx, nu_json_object_t json)
{
    transform_t *transform = (transform_t*)data;
    nuecs_entity_reference_deserialize_json_object(ctx, json, "child", &transform->child);
    nuecs_entity_reference_deserialize_json_object(ctx, json, "next", &transform->next);
    nuecs_entity_reference_deserialize_json_object(ctx, json, "parent", &transform->parent);
    nuecs_entity_reference_deserialize_json_object(ctx, json, "prev", &transform->prev);
    nu_json_value_as_bool(nu_json_object_get_by_name(json, "dirty"), &transform->dirty);
    return NU_SUCCESS;
}

static uint32_t nu_random()
{
    static uint32_t next = 1231125;
    next = (unsigned int)(next * 123423542) % 23423425;
    return next;
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

    /* load sdf interface */
    nu_module_t renderer_module = nu_renderer_get_module();
    nuvk_renderer_interface_load(renderer_module);

    /* load lua interface */
    NU_ASSERT(nulua_interface_load_all(lua_module) == NU_SUCCESS);
    nulua_plugin_t plugin;
    NU_ASSERT(nulua_plugin_load("$ENGINE/script/test.lua", &plugin));
    NU_ASSERT(nulua_plugin_load("$ENGINE/script/spectator.lua", &plugin));

    /* load ecs interface */
    nuecs_interface_load_all(ecs_module);

    /**************/
    /* COMPONENTS */
    /**************/

    nuecs_component_t position_component, health_component, velocity_component, score_component, transform_component;
    {
        nuecs_component_info_t info;
        memset(&info, 0, sizeof(nuecs_component_info_t));
        info.name             = "position";
        info.size             = sizeof(position_t);
        info.serialize_json   = serialize_position;
        info.flags            = NUECS_COMPONENT_FLAG_SERIALIZABLE;
        nuecs_component_build(&info, &position_component);
    }
    {
        nuecs_component_info_t info;
        memset(&info, 0, sizeof(nuecs_component_info_t));
        info.name             = "health";
        info.size             = sizeof(health_t);
        info.serialize_json   = serialize_health;
        info.flags            = NUECS_COMPONENT_FLAG_SERIALIZABLE;
        nuecs_component_build(&info, &health_component);
    }
    {
        nuecs_component_info_t info;
        memset(&info, 0, sizeof(nuecs_component_info_t));
        info.name             = "velocity";
        info.size             = sizeof(velocity_t);
        info.serialize_json   = serialize_velocity;
        info.flags            = NUECS_COMPONENT_FLAG_SERIALIZABLE;
        nuecs_component_build(&info, &velocity_component);
    }
    {
        nuecs_component_info_t info;
        memset(&info, 0, sizeof(nuecs_component_info_t));
        info.name             = "score";
        info.size             = sizeof(score_t);
        info.serialize_json   = serialize_score;
        info.flags            = NUECS_COMPONENT_FLAG_SERIALIZABLE;
        nuecs_component_build(&info, &score_component);
    }
    {
        nuecs_component_info_t info;
        memset(&info, 0, sizeof(nuecs_component_info_t));
        info.name             = "transform";
        info.size             = sizeof(transform_t);
        info.serialize_json   = serialize_transform;
        info.deserialize_json = deserialize_transform;
        info.flags            = NUECS_COMPONENT_FLAG_SERIALIZABLE;
        nuecs_component_build(&info, &transform_component);
    }

    /***********/
    /* SYSTEMS */
    /***********/

    nuecs_system_t move_player_system;
    {
        nuecs_component_dependency_t dependency = {
            .component = velocity_component,
            .access = NUECS_COMPONENT_ACCESS_WRITE
        };
        nuecs_system_info_t info = {
            .name             = "move_player",
            .start            = move_player_start,
            .stop             = move_player_stop,
            .update           = move_player_update,
            .state_size       = sizeof(move_player_system_t),
            .dependency_count = 1,
            .dependencies     = &dependency
        };
        nuecs_system_build(&info, &move_player_system);
    }

    /*************/
    /* PIPELINES */
    /*************/

    nuecs_pipeline_t pipeline;
    {
        nuecs_system_t systems[] = {
            move_player_system
        };
        nuecs_pipeline_info_t info = {
            .name         = "main_pipeline",
            .systems      = systems,
            .system_count = 1
        };
        nuecs_pipeline_build(&info, &pipeline);
    }

    /*********/
    /* SCENE */
    /*********/

    nuecs_scene_t scene;
    nuecs_scene_create(&scene);
    nuecs_scene_set_pipeline(scene, pipeline);
    
    nuecs_entity_t teste;
    {
        nuecs_entity_info_t info;
        info.component_count = 1;
        info.components = &velocity_component;
        velocity_t vel = { .velocity = {1, 2, 3} };
        nuecs_component_data_ptr_t data[] = {&vel};
        info.component_data = data;
        nuecs_entity_create(scene, &info, &teste);
    }

    {
        nuecs_entity_t entity;
        transform_t transform = {
            .child = NU_NULL_HANDLE,
            .next = NU_NULL_HANDLE,
            .parent = NU_NULL_HANDLE,
            .prev = NU_NULL_HANDLE,
            .dirty = false
        };
        nuecs_entity_reference_bind(scene, &transform.child, teste);
        nuecs_component_data_ptr_t data[] = {
            &transform
        };
        nuecs_entity_info_t info = {
            .components = &transform_component,
            .component_count = 1,
            .component_data = data
        };
        nuecs_entity_create(scene, &info, &entity);

        velocity_t vel = { .velocity = {0, 1, 2} };
        nuecs_entity_add_component(scene, &entity, velocity_component, &vel);
        nuecs_entity_remove_component(scene, &entity, velocity_component);

        for (uint32_t i = 0; i < 5; i++) {
            nuecs_entity_info_t infos;
            infos.component_count = 0;
            nuecs_entity_t ent;
            nuecs_entity_create(scene, &infos, &ent);
        }

        nuecs_entity_reference_t ref = NU_NULL_HANDLE;
        nuecs_entity_reference_bind(scene, &ref, entity);

        nuecs_entity_t e;
        nuecs_entity_reference_resolve(scene, &ref, &e);

        nuecs_entity_destroy(scene, teste);
    }

    // nuecs_scene_save_json(scene, "$ROOT/save.json");
    nuecs_scene_clear(scene);
    nuecs_scene_load_json(scene, "$ROOT/save.json");

    nuecs_query_info_t qinfo;
    qinfo.include_components      = &transform_component;
    qinfo.include_component_count = 1;
    qinfo.exclude_components      = NULL;
    qinfo.exclude_component_count = 0;
    nuecs_query_t query;
    nuecs_query_create(scene, &qinfo, &query);
    nuecs_query_result_t result;
    nuecs_query_resolve(scene, query, &result);
    for (uint32_t i = 0; i < result.count; i++) {
        nu_info("test", "%d", result.chunks[i].count);
        // transform_t *transforms = (transform_t*)transforms;
        // for (uint32_t j = 0; j < chunks.views[i].count; j++) {
        //     nuecs_entity_t child;
        //     nuecs_entity_reference_resolve(scene, &transforms[j].child, &child);
        //     /* TODO: ajouter les fonctions de lecture de composants / get etc... */
        // }
    }

    nu_context_request_stop();

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