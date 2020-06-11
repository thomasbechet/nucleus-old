#ifndef NUVK_INSTANCE_H
#define NUVK_INSTANCE_H

#include "../renderer.h"

nu_result_t nuvk_instance_create(void);
nu_result_t nuvk_instance_destroy(void);
VkInstance nuvk_instance_get_handle(void);

#endif