#include <nucleus/module/ecs/plugin/query.h>

nu_result_t nuecs_query_create(nuecs_scene_t scene_handle, const nuecs_query_info_t *info, nuecs_query_t *handle)
{
    nuecs_scene_data_t *scene = (nuecs_scene_data_t*)scene_handle;

    return NU_SUCCESS;
}
nu_result_t nuecs_query_destroy(nuecs_scene_t scene, nuecs_query_t handle)
{
    return NU_SUCCESS;
}
nu_result_t nuecs_query_resolve_chunks(nuecs_scene_t scene, nuecs_query_t query, nuecs_query_chunks_t *chunks)
{
    chunks->count = 0;
    chunks->views = NULL;

    return NU_SUCCESS;
}
