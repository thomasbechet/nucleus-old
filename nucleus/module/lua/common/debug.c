#include <nucleus/module/lua/common/debug.h>

#include <nucleus/module/lua/common/logger.h>

nu_result_t nulua_debug_stack(lua_State *L)
{
    nu_string_t dump;
    nu_string_allocate(&dump);
    uint32_t top = lua_gettop(L);
    for (uint32_t i = 1; i <= top; i++) {
        nu_string_t type;
        int type_id = lua_type(L, i);
        nu_string_allocate_format(&type, "[%ld] %s ", i, lua_typename(L, type_id));
        nu_string_append(&dump, type);
        nu_string_free(type);
    }
    nu_info(NULUA_LOGGER_NAME, "STACK: %s", nu_string_get_cstr(dump));
    nu_string_free(dump);

    return NU_SUCCESS;
}