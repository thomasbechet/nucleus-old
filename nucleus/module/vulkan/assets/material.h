#ifndef NUVK_ASSETS_MATERIAL_H
#define NUVK_ASSETS_MATERIAL_H

#include <nucleus/nucleus.h>

typedef struct {
    nu_vec3f_t color;
    uint32_t buffer_id;
    uint32_t id;
} nuvk_material_data_t;

#endif