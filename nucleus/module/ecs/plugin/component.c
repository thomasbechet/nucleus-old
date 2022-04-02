#include <nucleus/module/ecs/plugin/component.h>

nu_result_t nuecs_component_initialize(
    nuecs_component_data_t *component,
    const nuecs_component_info_t *info
)
{
    nu_string_allocate_cstr(&component->name, info->name);
    component->size             = info->size;
    component->serialize_json   = info->serialize_json;
    component->deserialize_json = info->deserialize_json; 

    return NU_SUCCESS;
}
nu_result_t nuecs_component_terminate(
    nuecs_component_data_t *component
)
{
    nu_string_free(component->name);

    return NU_SUCCESS;
}