#include "context.h"

#include "../common/logger.h"
#include "instance.h"
#include "debug_messenger.h"
#include "surface.h"
#include "physical_device.h"
#include "device.h"
#include "swapchain.h"

static nuvk_context_t _data;

nu_result_t nuvk_context_create(void)
{
    nu_result_t result;
    result = NU_SUCCESS;

    /* create instance */
    nu_info(NUVK_VULKAN_LOG_NAME"Context -- Creating instance...\n");
    result = nuvk_instance_create(&_data.instance);
    if (result != NU_SUCCESS) return result;

    /* create debug messenger */
    nu_info(NUVK_VULKAN_LOG_NAME"Context -- Creating debug messenger...\n");
#ifdef NU_DEBUG
    result = nuvk_debug_messenger_create(&_data.debug_messenger, _data.instance);
    if (result != NU_SUCCESS) return result;
#endif

    /* create surface */
    nu_info(NUVK_VULKAN_LOG_NAME"Context -- Creating surface...\n");
    result = nuvk_surface_create(&_data.surface, _data.instance);
    if (result != NU_SUCCESS) return result;

    /* pick physical device */
    nu_info(NUVK_VULKAN_LOG_NAME"Context -- Picking physical device...\n");
    result = nuvk_physical_device_pick(&_data.physical_device, &_data.queue_family_indices, _data.instance, _data.surface);
    if (result != NU_SUCCESS) return result;

    /* create device (and recover queues) */
    nu_info(NUVK_VULKAN_LOG_NAME"Context -- Creating device...\n");
    result = nuvk_device_create(&_data.device, &_data.queues, _data.surface, _data.physical_device);
    if (result != NU_SUCCESS) return result;

    /* create swapchain */
    nu_info(NUVK_VULKAN_LOG_NAME"Context -- Creating swapchain...\n");
    result = nuvk_swapchain_create(&_data.swapchain, _data.surface, _data.physical_device, _data.device);
    if (result != NU_SUCCESS) return result;

    return result;
}
nu_result_t nuvk_context_destroy(void)
{
    /* destroy swapchain */
    nuvk_swapchain_destroy(_data.swapchain, _data.device);

    /* destroy device */
    nuvk_device_destroy(_data.device);

    /* destroy surface */
    nuvk_surface_destroy(_data.surface, _data.instance);

    /* destroy debug messenger */
#ifdef NU_DEBUG
    nuvk_debug_messenger_destroy(_data.debug_messenger, _data.instance);
#endif

    /* destroy instance */
    nuvk_instance_destroy(_data.instance);

    return NU_SUCCESS;
}
nu_result_t nuvk_context_update_swapchain(void)
{
    vkDeviceWaitIdle(_data.device);
}
const nuvk_context_t *nuvk_context_get(void)
{
    return &_data;
}