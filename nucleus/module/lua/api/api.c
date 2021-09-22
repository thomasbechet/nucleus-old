#include <nucleus/module/lua/api/api.h>

#include <nucleus/module/lua/api/input.h>
#include <nucleus/module/lua/api/context.h>
#include <nucleus/module/lua/api/vec.h>

nu_result_t nulua_register_api(lua_State *L)
{
    nulua_register_context_api(L);
    nulua_register_input_api(L);
    nulua_register_vec_api(L);

    return NU_SUCCESS;
}