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

static bool nuecs_system_cmp(const void *a, const void *b)
{
    return *((nuecs_system_t*)a) == *((nuecs_system_t*)b);
}
nu_result_t nuecs_pipeline_initialize(nuecs_pipeline_data_t *pipeline, const nuecs_pipeline_info_t *info)
{
    /* allocate */
    nu_string_allocate_cstr(&pipeline->name, info->name);
    nu_array_allocate(&pipeline->systems, sizeof(nuecs_system_t));
    nu_array_allocate_capacity(&pipeline->sequence, sizeof(uint32_t), info->system_count);
    
    /* save systems */
    for (uint32_t i = 0; i < info->system_count; i++) {
        /* check unique */
        uint32_t index;
        if (!nu_array_find_index(pipeline->systems, nuecs_system_cmp, &info->systems[i], &index)) {
            index = nu_array_get_size(pipeline->systems);
            nu_array_push(pipeline->systems, &info->systems[i]);
        }
        /* append to sequence */
        nu_array_push(pipeline->sequence, &index);
    }
    
    return NU_SUCCESS;
}
nu_result_t nuecs_pipeline_terminate(nuecs_pipeline_data_t *pipeline)
{
    /* free */
    nu_array_free(pipeline->sequence);
    nu_array_free(pipeline->systems);
    nu_string_free(pipeline->name);
    
    return NU_SUCCESS;
}

nu_result_t nuecs_pipeline_instance_initialize(
    nuecs_pipeline_instance_data_t *instance,
    nuecs_system_manager_data_t *manager,
    nuecs_pipeline_t pipeline
)
{
    /* get pipeline info */
    nuecs_pipeline_data_t *pipeline_data;
    uint32_t pipeline_index;
    NU_HANDLE_GET_ID(pipeline, pipeline_index);
    nu_array_get(manager->pipelines, pipeline_index, &pipeline_data);

    /* initialize instance */
    instance->pipeline = pipeline;   
    nu_array_allocate_capacity(&instance->system_instances, sizeof(nuecs_system_instance_t), nu_array_get_size(pipeline_data->systems));
    
    /* create system instance */
    nuecs_system_t *systems; uint32_t system_count;
    nu_array_get_data(pipeline_data->systems, &systems, &system_count);
    for (uint32_t i = 0; i < system_count; i++) {
        /* get system info */
        nuecs_system_data_t *system_data;
        uint32_t system_index;
        NU_HANDLE_GET_ID(systems[i], system_index);
        nu_array_get(manager->systems, system_index, &system_data);
        /* create system instance */
        nuecs_system_instance_t *system_instance;
        nu_array_push(instance->system_instances, NULL);
        nu_array_get_last(instance->system_instances, &system_instance);
        system_instance->start  = system_data->start;
        system_instance->stop   = system_data->stop;
        system_instance->update = system_data->update;
        system_instance->state  = nu_malloc(sizeof(system_data->state_size));
    }

    return NU_SUCCESS;
}
nu_result_t nuecs_pipeline_instance_terminate(nuecs_pipeline_instance_data_t *instance)
{
    /* free system instances */
    nuecs_system_instance_t *system_instances; uint32_t system_instance_count;
    nu_array_get_data(instance->system_instances, &system_instances, &system_instance_count);
    for (uint32_t i = 0; i < system_instance_count; i++) {
        nu_free(system_instances[i].state);
    }

    /* free */
    nu_array_free(instance->system_instances);

    return NU_SUCCESS;
}
nu_result_t nuecs_pipeline_instance_start(nuecs_pipeline_instance_data_t *instance, nuecs_scene_t scene)
{
    nuecs_system_instance_t *system_instances; uint32_t system_instance_count;
    nu_array_get_data(instance->system_instances, &system_instances, &system_instance_count);
    for (uint32_t i = 0; i < system_instance_count; i++) {
        system_instances[i].start(system_instances[i].state, scene);
    }
    return NU_SUCCESS;
}
nu_result_t nuecs_pipeline_instance_stop(nuecs_pipeline_instance_data_t *instance, nuecs_scene_t scene)
{
    nuecs_system_instance_t *system_instances; uint32_t system_instance_count;
    nu_array_get_data(instance->system_instances, &system_instances, &system_instance_count);
    for (uint32_t i = 0; i < system_instance_count; i++) {
        system_instances[i].stop(system_instances[i].state, scene);
    }
    return NU_SUCCESS;
}
nu_result_t nuecs_pipeline_instance_update(
    nuecs_pipeline_instance_data_t *instance,
    nuecs_system_manager_data_t *manager,
    nuecs_scene_t scene
)
{
    /* get pipeline info */
    nuecs_pipeline_data_t *pipeline;
    uint32_t pipeline_index;
    NU_HANDLE_GET_ID(instance->pipeline, pipeline_index);
    nu_array_get(manager->pipelines, pipeline_index, &pipeline);

    /* get system instances */
    nuecs_system_instance_t *system_instances; uint32_t system_instance_count;
    nu_array_get_data(instance->system_instances, &system_instances, &system_instance_count);

    /* get sequence */
    uint32_t *sequence; uint32_t sequence_length;
    nu_array_get_data(pipeline->sequence, &sequence, &sequence_length);
    
    /* update systems */
    for (uint32_t i = 0; i < sequence_length; i++) {
        /* update */
        const nuecs_system_instance_t *si = &system_instances[sequence[i]];
        si->update(si->state, scene);
    }

    return NU_SUCCESS;
}