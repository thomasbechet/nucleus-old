#include <nucleus/module/utils/loader/loader.h>

#define NUUTILS_LOGGER_NAME "[LOADER] "

nu_result_t nuutils_load_mesh_from_obj(const char *filename, nu_renderer_mesh_t *handle)
{    
    nu_path_t path;
    nu_path_allocate_cstr(filename, &path);

    // nu_renderer_submesh_info_t submeshInfo = {0};
    // submeshInfo.vertex_count     = first->second.positions.size();
    // submeshInfo.positions        = (nu_vec3f_t*)first->second.positions.data();
    // submeshInfo.uvs              = (nu_vec2f_t*)first->second.uvs.data();
    // submeshInfo.colors           = (nu_vec3f_t*)first->second.colors.data();
    // submeshInfo.indice_count     = 0;
    // nu_renderer_mesh_create_info_t info = {0};
    // info.submeshes     = &submeshInfo;
    // info.submesh_count = 1;

    // if (nu_renderer_mesh_create(&info, handle) != NU_SUCCESS) {
    //     nu_warning(NUUTILS_LOGGER_NAME"Failed to load mesh.\n");
    //     return NU_FAILURE;
    // }

    nu_path_free(path);

    return NU_FAILURE;
}
nu_result_t nuutils_load_texture(const char *filename, nu_renderer_texture_t *handle)
{
    nu_path_t path;
    nu_path_allocate_cstr(filename, &path);

    nu_path_free(path);

    return NU_FAILURE;
}