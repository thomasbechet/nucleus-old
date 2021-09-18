#include <nucleus/module/utils/command/command.h>

#define NUUTILS_LOGGER_NAME "COMMAND"

typedef struct {
    nu_event_id_t event_id;
} nuutils_module_data_t;

static nuutils_module_data_t _module;

static nu_result_t nuutils_command_event_initialize(void *data)
{
    nuutils_command_event_t *event = (nuutils_command_event_t*)data;
    (void)event;

    return NU_SUCCESS;
}
static nu_result_t nuutils_command_event_terminate(void *data)
{
    nuutils_command_event_t *event = (nuutils_command_event_t*)data;
    nu_string_array_free(event->tokens);

    return NU_SUCCESS;
}

nu_result_t nuutils_command_plugin_initialize(void)
{
    nu_result_t result;

    /* register command event */
    nu_event_register_info_t info;
    info.initialize = nuutils_command_event_initialize;
    info.terminate  = nuutils_command_event_terminate;
    info.size       = sizeof(nuutils_command_event_t);
    result = nu_event_register(&info, &_module.event_id);
    NU_CHECK(result == NU_SUCCESS, return result, NUUTILS_LOGGER_NAME, "Failed to initialize command event.");

    return NU_SUCCESS;
}
nu_result_t nuutils_command_plugin_terminate(void)
{
    return NU_SUCCESS;
}

nu_result_t nuutils_command_get_event_id(nu_event_id_t *id)
{
    *id = _module.event_id;
    return NU_SUCCESS;
}
nu_result_t nuutils_command_execute(const char *cstr_command)
{
    /* split command */
    nu_string_array_t tokens;
    nu_string_array_allocate(&tokens);
    nu_string_cstr_split(cstr_command, " ", tokens);

    /* post event */
    nuutils_command_event_t event;
    event.tokens = tokens;
    nu_event_post(_module.event_id, &event);

    /* handle built-in commands */
    uint32_t count = nu_string_array_get_length(tokens);
    if (count == 2 && NU_MATCH(nu_string_array_get(tokens, 0), "fov")) {
        float fov = atof(nu_string_array_get(tokens, 1));
        nu_renderer_camera_set_fov(0, fov);
    }

    if (count == 3 && NU_MATCH(nu_string_array_get(tokens, 0), "viewport")) {
        nu_vec2u_t size = {
            (uint32_t)atoi(nu_string_array_get(tokens, 1)),
            (uint32_t)atoi(nu_string_array_get(tokens, 2))
        };
        nu_renderer_viewport_set_size(size);
    }

    if (count == 2 && NU_MATCH(nu_string_array_get(tokens, 0), "window")) {
        const char *token1 = nu_string_array_get(tokens, 1);
        if (NU_MATCH(token1, "fullscreen")) {
            nu_window_set_mode(NU_WINDOW_MODE_FULLSCREEN);
        } else if (NU_MATCH(token1, "windowed")) {
            nu_window_set_mode(NU_WINDOW_MODE_WINDOWED);
        } else if (NU_MATCH(token1, "borderless")) {
            nu_window_set_mode(NU_WINDOW_MODE_BORDERLESS);
        }
    }

    if (count == 1 && NU_MATCH(nu_string_array_get(tokens, 0), "shutdown")) {
        nu_context_request_stop();
    }

    return NU_SUCCESS;
}