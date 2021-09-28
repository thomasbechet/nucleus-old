#ifndef NUVK_MODULE_INTERFACE_H
#define NUVK_MODULE_INTERFACE_H

#include <nucleus/module/vulkan/sdf/scene/interface.h>

#define NUVK_MODULE_NAME "nucleus-vulkan"
#define NUVK_MODULE_ID 0x8

#define NUVK_SDF_INTERFACE_NAME "nuvk_sdf_interface"

typedef struct {
    nu_result_t (*register_instance_type)(const nuvk_sdf_instance_type_info_t*, nuvk_sdf_instance_type_t*);
    nu_result_t (*get_instance_type)(nuvk_sdf_instance_type_primitives_t, nuvk_sdf_instance_type_t*);
    nu_result_t (*create_instance)(const nuvk_sdf_instance_info_t*, nuvk_sdf_instance_t*);
    nu_result_t (*destroy_instance)(nuvk_sdf_instance_t);
    nu_result_t (*update_instance_transform)(nuvk_sdf_instance_t, const nu_transform_t*);
    nu_result_t (*update_instance_data)(nuvk_sdf_instance_t, const void*);
} nuvk_sdf_interface_t;

#endif