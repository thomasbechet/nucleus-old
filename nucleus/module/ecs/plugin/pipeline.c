#include <nucleus/module/ecs/plugin/pipeline.h>

#include <nucleus/module/ecs/plugin/system.h>

// static bool nuecs_pipeline_node_cmp(const void *a, const void *b)
// {
//     nuecs_pipeline_node_t **na = (nuecs_pipeline_node_t**)a;
//     nuecs_pipeline_node_t **nb = (nuecs_pipeline_node_t**)b;
//     return (*na) == (*nb);
// }
// static nu_result_t nuecs_pipeline_process_nodes(nu_array_t nodes, nuecs_pipeline_node_t *node)
// {
//     /* iterate over dependencies */
//     for (uint32_t i = 0; i < node->dependency_count; i++) {
//         nuecs_pipeline_node_t *dependency = &node->dependencies[i];
        
//         /* check if this node has already be processed */
//         if (!nu_array_has(nodes, nuecs_pipeline_node_cmp, &dependency)) {

//             /* go deeper in the graph */
//             nuecs_pipeline_process_nodes(nodes, dependency);
//         }
//     }

//     /* all dependencies have been processed */
//     nu_array_push(nodes, &node);

//     return NU_SUCCESS;
// }

nu_result_t nuecs_pipeline_initialize(nuecs_pipeline_data_t *pipeline, const nuecs_pipeline_info_t *info)
{
    /* name */
    nu_string_allocate_cstr(&pipeline->name, info->name);
    
    /* systems */
    nu_array_allocate(&pipeline->start_systems, sizeof(nuecs_system_data_t*));
    nu_array_allocate(&pipeline->load_systems, sizeof(nuecs_system_data_t*));
    nu_array_allocate(&pipeline->update_systems, sizeof(nuecs_system_data_t*));
    nu_array_allocate(&pipeline->unload_systems, sizeof(nuecs_system_data_t*));
    nu_array_allocate(&pipeline->stop_systems, sizeof(nuecs_system_data_t*));
    
    return NU_SUCCESS;
}
nu_result_t nuecs_pipeline_terminate(nuecs_pipeline_data_t *pipeline)
{
    /* systems */
    nu_array_free(pipeline->stop_systems);
    nu_array_free(pipeline->unload_systems);
    nu_array_free(pipeline->update_systems);
    nu_array_free(pipeline->load_systems);
    nu_array_free(pipeline->start_systems);

    /* name */
    nu_string_free(pipeline->name);
    
    return NU_SUCCESS;
}

nu_result_t nuecs_pipeline_instance_initialize(nuecs_pipeline_instance_data_t *instance, const nuecs_pipeline_data_t *pipeline)
{
    return NU_SUCCESS;   
}
nu_result_t nuecs_pipeline_instance_terminate(nuecs_pipeline_instance_data_t *instance)
{
    return NU_SUCCESS;
}
nu_result_t nuecs_pipeline_instance_start_stage(nuecs_pipeline_instance_data_t *instance, nuecs_scene_t scene)
{
    return NU_SUCCESS;
}
nu_result_t nuecs_pipeline_instance_load_stage(nuecs_pipeline_instance_data_t *instance, nuecs_scene_t scene)
{
    return NU_SUCCESS;
}
nu_result_t nuecs_pipeline_instance_update_stage(nuecs_pipeline_instance_data_t *instance, nuecs_scene_t scene)
{
    return NU_SUCCESS;
}
nu_result_t nuecs_pipeline_instance_store_stage(nuecs_pipeline_instance_data_t *instance, nuecs_scene_t scene)
{
    return NU_SUCCESS;
}
nu_result_t nuecs_pipeline_instance_stop_stage(nuecs_pipeline_instance_data_t *instance, nuecs_scene_t scene)
{
    return NU_SUCCESS;
}