#ifndef NUVK_MODULE_INTERFACE_H
#define NUVK_MODULE_INTERFACE_H

#include <nucleus/module/vulkan/sdf/scene/interface.h>

#define NUVK_MODULE_NAME "nucleus-vulkan"
#define NUVK_MODULE_ID 0x8

#define NUVK_SDF_INTERFACE_NAME "nuvk_sdf_interface"

typedef struct {
    nu_result_t (*instance_type_register)(const nuvk_sdf_instance_type_info_t*, nuvk_sdf_instance_type_t*);
    nu_result_t (*instance_create)(const nuvk_sdf_instance_info_t*, nuvk_sdf_instance_t*);
    nu_result_t (*instance_destroy)(nuvk_sdf_instance_t);
    nu_result_t (*instance_update_transform)(nuvk_sdf_instance_t, const nu_transform_t*);
    nu_result_t (*instance_update_data)(nuvk_sdf_instance_t, const void*);
} nuvk_sdf_interface_t;

#endif