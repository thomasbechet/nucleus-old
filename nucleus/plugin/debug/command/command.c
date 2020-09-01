#include "command.h"

nu_result_t nudebug_plugin_command_initialize(void)
{
    nu_info("PLUGIN INITIALIZE\n");
    return NU_SUCCESS;
}
nu_result_t nudebug_plugin_command_terminate(void)
{
    nu_info("PLUGIN TERMINATE\n");
    return NU_SUCCESS;
}
nu_result_t nudebug_plugin_command_update(void)
{
    return NU_SUCCESS;
}