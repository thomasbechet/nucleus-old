#include <nucleus/module/utils/module/implementation.h>

#include <nucleus/module/utils/loader/loader.h>
#include <nucleus/module/utils/command/command.h>

/* loader interface */
nu_result_t nuutils_loader_load_mesh_from_obj_impl(const char* path, nu_renderer_mesh_t* handle)
{
    return nuutils_loader_load_mesh_from_obj(path, handle);
}
nu_result_t nuutils_loader_load_texture_impl(const char* path, nu_renderer_texture_t* handle)
{
    return nuutils_loader_load_texture(path, handle);
}
/* command interface */
nu_result_t nuutils_command_get_event_id_impl(nu_event_id_t* id)
{
    return nuutils_command_get_event_id(id);
}
nu_result_t nuutils_command_execute_impl(const char* command)
{
    return nuutils_command_execute(command);
}
/* command plugin */
nu_result_t nuutils_command_plugin_initialize_impl(void)
{
    return nuutils_command_plugin_initialize();
}
nu_result_t nuutils_command_plugin_terminate_impl(void)
{
    return nuutils_command_plugin_terminate();
}
nu_result_t nuutils_command_plugin_update_impl(void)
{
    return NU_SUCCESS;
}