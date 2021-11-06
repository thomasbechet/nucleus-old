#include <nucleus/module/lua/api/vulkan/renderer.h>

#include <nucleus/module/lua/common/logger.h>
#include <nucleus/module/vulkan.h>
#include <lauxlib.h>

typedef struct {
    nuvk_renderer_interface_t interface;
} nulua_module_data_t;

static nulua_module_data_t _module;

static int VkRenderer_material_create(lua_State *L)
{
    nuvk_material_info_t info;
    memset(&info, 0, sizeof(nuvk_material_info_t));
    return 0;
}

nu_result_t nulua_register_vulkan_renderer_api(lua_State *L)
{
    nu_module_t renderer = nu_renderer_get_module();
    nu_result_t result = nu_module_get_interface(renderer, NUVK_RENDERER_INTERFACE_NAME, &_module.interface);
    NU_CHECK(result == NU_SUCCESS, return NU_FAILURE, NULUA_LOGGER_NAME, "Failed to get the interface.");

    static const struct luaL_Reg VkRenderer_methods[] = {
        {"material_create", VkRenderer_material_create},
        {NULL, NULL}
    };

    luaL_newlib(L, VkRenderer_methods);
    lua_setglobal(L, "VkRenderer");
    return NU_SUCCESS;
}