#include <nucleus/module/ecs/plugin/system.h>

nu_result_t nuecs_system_initialize(
    nuecs_system_data_t *system,
    const nuecs_system_info_t *info
)
{
    nu_string_allocate_cstr(&system->name, info->name);
    system->initialize = info->initialize;
    system->terminate  = info->terminate;
    system->invoke     = info->invoke;
    system->state_size = info->state_size;
    return NU_SUCCESS;
}
nu_result_t nuecs_system_terminate(
    nuecs_system_data_t *system
)
{
    nu_string_free(system->name);
    return NU_SUCCESS;
}

// nu_result_t nuecs_pipeline_initialize(
//     nuecs_pipeline_data_t *pipeline,
//     const nuecs_pipeline_info_t *info
// )
// {
//     /* TODO: detect cyclic dependencies */

//     /* allocate systems handles */
//     nu_array_allocate(&pipeline->systems, sizeof(nuecs_system_t));

//     /* allocate working node array */
//     nu_array_t node_array;
//     nu_array_allocate(&node_array, sizeof(nuecs_pipeline_node_t*));

//     /* process list of nodes */
//     for (uint32_t i = 0; i < info->target_count; i++) {
//         nuecs_pipeline_process_nodes(node_array, &info->targets[i]);
//     }

//     /* compute list of systems */
//     nuecs_pipeline_node_t **nodes; uint32_t node_count;
//     nu_array_get_data(node_array, &nodes, &node_count);
//     for (uint32_t i = 0; i < node_count; i++) {
//         if (nodes[i]->system != NU_NULL_HANDLE) {
//             nu_array_push(pipeline->systems, &nodes[i]->system);
//         }
//     }

//     /* free resources */
//     nu_array_free(node_array);

//     return NU_SUCCESS;
// }
// nu_result_t nuecs_pipeline_terminate(
//     nuecs_pipeline_data_t *pipeline
// )
// {
//     nu_array_free(pipeline->systems);

//     return NU_SUCCESS;
// }