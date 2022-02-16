#include <nucleus/module/lua/api/vulkan/renderer.h>

#include <nucleus/module/lua/common/logger.h>
#define NUVK_LOADER_IMPLEMENTATION
#include <nucleus/module/vulkan.h>
#include <lauxlib.h>

static int VkRenderer_material_create(lua_State *L)
{
    nuvk_material_info_t info;
    memset(&info, 0, sizeof(nuvk_material_info_t));
    return 0;
}

nu_result_t nulua_register_vulkan_renderer_api(lua_State *L)
{
    NU_CHECK(nuvk_renderer_interface_load(nu_renderer_get_module()) == NU_SUCCESS, return NU_FAILURE,
        NULUA_LOGGER_NAME, "Failed to load vulkan interface.");

    static const struct luaL_Reg VkRenderer_methods[] = {
        {"material_create", VkRenderer_material_create},
        {NULL, NULL}
    };

    luaL_newlib(L, VkRenderer_methods);
    lua_setglobal(L, "VkRenderer");
    return NU_SUCCESS;
}