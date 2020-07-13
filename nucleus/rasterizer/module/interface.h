#ifndef NURZ_INTERFACE_H
#define NURZ_INTERFACE_H

#include "../../core/nucleus_core.h"

#define NURZ_MODULE_NAME "nucleus-rasterizer"
#define NURZ_MODULE_ID 0x7

#define NURZ_RENDERER_INTERFACE_NAME        "nurz_renderer_interface"
#define NURZ_RENDERER_INTERFACE_LOADER_NAME "nurz_renderer_interface_loader"

typedef struct {
    
} nurz_renderer_interface_t;

typedef nu_result_t (*nurz_renderer_interface_loader_pfn_t)(nurz_renderer_interface_t*);

#endif