#ifndef NUVK_INSTANCE_H
#define NUVK_INSTANCE_H

#include "context.h"

nu_result_t nuvk_instance_create(VkInstance *instance);
nu_result_t nuvk_instance_destroy(VkInstance instance);

#endif