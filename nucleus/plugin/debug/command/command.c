#include "command.h"

nu_result_t nudebug_plugin_command_initialize(void)
{
    nu_info("INITIALIZE COMMAND\n");
    return NU_SUCCESS;
}
nu_result_t nudebug_plugin_command_terminate(void)
{
    nu_info("TERMINATE COMMAND\n");
    return NU_SUCCESS;
}
nu_result_t nudebug_plugin_command_update(void)
{
    return NU_SUCCESS;
}