#include "command.hpp"

#include "logger.hpp"

typedef struct {
    nu_event_id_t event_id;
} nuutils_command_data_t;

static nuutils_command_data_t _data;

static nu_result_t nuutils_command_event_initialize(void *data)
{
    return NU_SUCCESS;
}
static nu_result_t nuutils_command_event_terminate(void *data)
{
    return NU_SUCCESS;
}

nu_result_t nuutils_plugin_command_initialize(void)
{
    /* create event */
    nu_event_register_info_t info;
    info.size       = sizeof(nuutils_command_event_t);
    info.initialize = nuutils_command_event_initialize;
    info.terminate  = nuutils_command_event_terminate;
    if (nu_event_register(&_data.event_id, &info) != NU_SUCCESS) {
        nu_warning(NUUTILS_LOGGER_NAME"Failed to register event.\n");
        return NU_FAILURE;
    }

    return NU_SUCCESS;
}
nu_result_t nuutils_plugin_command_terminate(void)
{
    return NU_SUCCESS;
}

nu_result_t nuutils_command_get_event_id(nu_event_id_t *id)
{
    *id = _data.event_id;

    return NU_SUCCESS;
}
nu_result_t nuutils_command_execute(const char *command)
{
    

    return NU_SUCCESS;
}

nu_result_t nuutils_command_get_interface(nuutils_command_interface_t *interface)
{
    interface->get_event_id = nuutils_command_get_event_id;
    interface->execute      = nuutils_command_execute;

    return NU_SUCCESS;
}