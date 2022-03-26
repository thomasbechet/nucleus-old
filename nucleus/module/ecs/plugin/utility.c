#include <nucleus/module/ecs/plugin/utility.h>

nu_result_t nuecs_sanatize_components(
    nuecs_component_data_t **in_components,
    uint32_t in_component_count,
    nuecs_component_data_t **components,
    uint32_t *component_count
)
{
    /* copy list */
    for (uint32_t i = 0; i < in_component_count; i++) {
        components[i] = in_components[i];
    }
    *component_count = in_component_count;
    /* TODO: remove duplicated components */

    return NU_SUCCESS;
}

bool nuecs_is_subset(const uint32_t *a, uint32_t ac, const uint32_t *b, uint32_t bc) 
{
    bool has_all = true;
    for (uint32_t i = 0; i < ac; i++) {
        bool found = false;
        for (uint32_t j = 0; j < bc; j++) {
            if (a[i] == b[j]) {
                found = true;
                break;
            }
        }
        if (!found) {
            has_all = false;
            break;
        }
    }
    return has_all;
}