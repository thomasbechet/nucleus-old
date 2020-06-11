#ifndef NUVK_DEVICE_H
#define NUVK_DEVICE_H

#include "../renderer.h"

typedef struct {
    VkQueue graphics;
    VkQueue presentation;
} nuvk_queues_t;

nu_result_t nuvk_device_create(void);
nu_result_t nuvk_device_destroy(void);

VkDevice nuvk_device_get_handle(void);
nuvk_queues_t nuvk_device_get_queues(void);

#endif