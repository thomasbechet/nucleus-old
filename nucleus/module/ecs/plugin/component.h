#ifndef NUECS_COMPONENT_H
#define NUECS_COMPONENT_H

#include <nucleus/module/ecs/plugin/types_public.h>

typedef struct {
    uint32_t size;
    nu_string_t name;
    uint8_t flags;
    nuecs_component_serialize_json_pfn_t serialize_json;
    nuecs_component_deserialize_json_pfn_t deserialize_json;
} nuecs_component_data_t;

nu_result_t nuecs_component_initialize(
    nuecs_component_data_t *component,
    const nuecs_component_info_t *info
);
nu_result_t nuecs_component_terminate(
    nuecs_component_data_t *component
);

#endif