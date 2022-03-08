#ifndef NUECS_UTILITY_H
#define NUECS_UTILITY_H

#include <nucleus/module/ecs/plugin/types_private.h>

nu_result_t nuecs_sanatize_components(
    nuecs_component_data_t **in_components,
    uint32_t in_component_count,
    nuecs_component_data_t **components,
    uint32_t *component_count
);

bool nuecs_is_subset(const uint32_t *a, uint32_t ac, const uint32_t *b, uint32_t bc);

#endif