#include <nucleus/module/lua/module/implementation.h>

#include <nucleus/module/lua/plugin/plugin.h>

/* plugin interface */
nu_result_t nulua_plugin_load_impl(const char* path, nulua_plugin_t* handle)
{
    return nulua_manager_load_plugin(path, handle);
}
/* manager plugin */
nu_result_t nulua_manager_plugin_initialize_impl(void)
{
    return nulua_manager_plugin_initialize();
}
nu_result_t nulua_manager_plugin_terminate_impl(void)
{
    return nulua_manager_plugin_terminate();
}
nu_result_t nulua_manager_plugin_update_impl(void)
{
    return nulua_manager_plugin_update();
}