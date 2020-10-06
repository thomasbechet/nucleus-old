#ifndef NUVK_DEBUG_MESSENGER_H
#define NUVK_DEBUG_MESSENGER_H

#include "context.h"

nu_result_t nuvk_debug_messenger_create(VkDebugUtilsMessengerEXT *debug_messenger, VkInstance instance);
nu_result_t nuvk_debug_messenger_destroy(VkDebugUtilsMessengerEXT debug_messenger, VkInstance instance);

#endif