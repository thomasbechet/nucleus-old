#include <nucleus/module/utils/command/command.hpp>

#include <nucleus/module/utils/command/logger.hpp>

#include <string>
#include <vector>
#include <algorithm>
#include <functional>
#include <cctype>

typedef struct {
    nu_event_id_t event_id;
} nuutils_command_data_t;

static nuutils_command_data_t _data;

// String utility functions
static inline std::string &ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
    return s;
}
static inline std::string &rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
    return s;
}
static inline std::string &trim(std::string &s) {
    return ltrim(rtrim(s));
}
static std::vector<std::string> split(const std::string& str, const std::string& delim)
{
    std::vector<std::string> tokens;
    size_t prev = 0, pos = 0;
    do
    {
        pos = str.find(delim, prev);
        if (pos == std::string::npos) pos = str.length();
        std::string token = str.substr(prev, pos-prev);
        if (!token.empty()) tokens.push_back(token);
        prev = pos + delim.length();
    }
    while (pos < str.length() && prev < str.length());
    return tokens;
}

static nu_result_t nuutils_command_event_initialize(void *data)
{
    nuutils_command_event_t *event = (nuutils_command_event_t*)data;

    return NU_SUCCESS;
}
static nu_result_t nuutils_command_event_terminate(void *data)
{
    nuutils_command_event_t *event = (nuutils_command_event_t*)data;

    for (uint32_t i = 0; i < event->argc; i++) {
        nu_free(event->args[i]);
    }
    nu_free(event->args);

    return NU_SUCCESS;
}

nu_result_t nuutils_command_plugin_initialize(void)
{
    /* create event */
    nu_event_register_info_t info;
    info.initialize = nuutils_command_event_initialize;
    info.terminate  = nuutils_command_event_terminate;
    info.size       = sizeof(nuutils_command_event_t);
    if (nu_event_register(&_data.event_id, &info) != NU_SUCCESS) {
        nu_warning(NUUTILS_LOGGER_NAME"Failed to initialize command event.\n");
        return NU_FAILURE;
    }

    return NU_SUCCESS;
}
nu_result_t nuutils_command_plugin_terminate(void)
{
    return NU_SUCCESS;
}

nu_result_t nuutils_command_get_event_id(nu_event_id_t *id)
{
    *id = _data.event_id;
    return NU_SUCCESS;
}
nu_result_t nuutils_command_execute(const char *cstr_command)
{
    // Split command
    std::string command(cstr_command);
    std::vector<std::string> tokens = split(trim(command), " ");

    // Post event
    nuutils_command_event_t event;
    event.argc = tokens.size();
    event.args = (char**)nu_malloc(sizeof(char*) * event.argc);
    for (uint32_t i = 0; i < tokens.size(); i++) {
        std::string token = tokens.at(i);
        event.args[i] = (char*)nu_malloc(sizeof(char) * (token.size() + 1));
        strcpy(event.args[i], token.c_str());
    }
    nu_event_post(_data.event_id, &event);

    //Handle builtin commands
    if (tokens.size() == 2 && tokens.at(0) == "fov") {
        nu_renderer_camera_set_fov(0, nu_radian(std::atof(tokens.at(1).c_str())));
    }

    if (tokens.size() == 3 && tokens.at(0) == "viewport") {
        nu_vec2u_t size = {(uint32_t)std::atoi(tokens.at(1).c_str()), (uint32_t)std::atoi(tokens.at(2).c_str())};
        nu_renderer_viewport_set_size(size);
    }

    if (tokens.size() == 2 && tokens.at(0) == "window") {
        if (tokens.at(1) == "fullscreen") {
            nu_window_set_mode(NU_WINDOW_MODE_FULLSCREEN);
        } else if (tokens.at(1) == "windowed") {
            nu_window_set_mode(NU_WINDOW_MODE_WINDOWED);
        } else if (tokens.at(1) == "borderless") {
            nu_window_set_mode(NU_WINDOW_MODE_BORDERLESS);
        }
    }

    if (tokens.size() == 1 && tokens.at(0) == "shutdown") {
        nu_context_request_stop();
    }

    return NU_SUCCESS;
}