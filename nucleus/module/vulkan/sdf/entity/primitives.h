#ifndef NUVK_SDF_PRIMITIVES_H
#define NUVK_SDF_PRIMITIVES_H

#include <nucleus/module/vulkan/sdf/entity/interface.h>

typedef struct {
    nuvk_sdf_instance_type_t sphere;
} nuvk_sdf_primitives_t;

nu_result_t nuvk_sdf_primitives_register(nuvk_sdf_primitives_t *primitives);

#endif