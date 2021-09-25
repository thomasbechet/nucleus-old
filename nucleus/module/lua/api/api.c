#include <nucleus/module/lua/api/api.h>

#include <nucleus/module/lua/api/context.h>
#include <nucleus/module/lua/api/input.h>
#include <nucleus/module/lua/api/renderer.h>
#include <nucleus/module/lua/api/vec.h>
#include <nucleus/module/lua/api/quat.h>
#include <nucleus/module/lua/api/transform.h>
#include <nucleus/module/lua/api/mat.h>

nu_result_t nulua_register_api(lua_State *L)
{
    nulua_register_context_api(L);
    nulua_register_input_api(L);
    nulua_register_renderer_api(L);
    nulua_register_vec_api(L);
    nulua_register_quat_api(L);
    nulua_register_transform_api(L);
    nulua_register_mat_api(L);

    return NU_SUCCESS;
}