#ifndef NUSR_INTERFACE_H
#define NUSR_INTERFACE_H

#include <nucleus/nucleus.h>

/* module */
#define NUSR_MODULE_NAME "nucleus-softrast"
#define NUSR_MODULE_ID 0x4

/* renderer system */
#define NUSR_RENDERER_INTERFACE_NAME        "nusr_renderer_interface"
#define NUSR_RENDERER_INTERFACE_LOADER_NAME "nusr_renderer_get_interface"

typedef struct {
} nusr_renderer_interface_t;

typedef nu_result_t (*nusr_renderer_interface_loader_pfn_t)(nusr_renderer_interface_t*, const char*);

#endif