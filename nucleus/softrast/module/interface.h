#ifndef NUSR_INTERFACE_H
#define NUSR_INTERFACE_H

#include "../../core/nucleus_core.h"

#define NUSR_MODULE_NAME "nucleus-softrast"
#define NUSR_MODULE_ID 0x7

#define NUSR_RENDERER_INTERFACE_NAME        "nusr_renderer_interface"
#define NUSR_RENDERER_INTERFACE_LOADER_NAME "nusr_renderer_interface_loader"

typedef struct {
    
} nusr_renderer_interface_t;

typedef nu_result_t (*nusr_renderer_interface_loader_pfn_t)(nusr_renderer_interface_t*);

#endif