/* Generated file: DO NOT EDIT ! */
#ifndef NUVK_MODULE_DEFINITION_H
#define NUVK_MODULE_DEFINITION_H

#include <nucleus/nucleus.h>
#include <nucleus/module/vulkan/assets/interface.h>
#include <nucleus/module/vulkan/scene/interface.h>

/* module */
#define NUVK_MODULE_NAME "nucleus-vulkan"
#define NUVK_MODULE_ID 8

/* interface */
#define NUVK_RENDERER_INTERFACE_NAME "nuvk_renderer_interface"

typedef struct {
    nu_result_t (*sdf_create)(const nuvk_sdf_info_t*, nuvk_sdf_t*);
    nu_result_t (*sdf_destroy)(nuvk_sdf_t);
    nu_result_t (*sdf_get_primitive)(nuvk_sdf_primitives_t, nuvk_sdf_t*);
    nu_result_t (*material_create)(const nuvk_material_info_t*, nuvk_material_t*);
    nu_result_t (*material_destroy)(nuvk_material_t);
    nu_result_t (*sdf_instance_create)(const nuvk_sdf_instance_info_t*, nuvk_sdf_instance_t*);
    nu_result_t (*sdf_instance_destroy)(nuvk_sdf_instance_t);
    nu_result_t (*sdf_instance_update_transform)(nuvk_sdf_instance_t, const nuvk_sdf_transform_t*);
    nu_result_t (*sdf_instance_update_data)(nuvk_sdf_instance_t, const void*);
} nuvk_renderer_interface_t;


/* plugin */

#endif