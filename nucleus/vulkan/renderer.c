#include "renderer.h"

#include "glfw/interface.h"
#include "presentation/instance.h"
#include "presentation/debug_messenger.h"
#include "presentation/surface.h"
#include "presentation/physical_device.h"
#include "presentation/device.h"
#include "presentation/swapchain.h"

nu_result_t nuvk_renderer_initialize(void)
{
    nu_result_t result;
    result = NU_SUCCESS;

    /* load glfw interface */
    result = nuvk_glfw_load_interface();
    if (result != NU_SUCCESS) return result;

    /* create instance */
    result = nuvk_instance_create();
    if (result != NU_SUCCESS) return result;

    /* create debug messenger */
#ifdef NU_DEBUG
    result = nuvk_debug_messenger_create(); 
    if (result != NU_SUCCESS) return result;
#endif

    /* create surface */
    result = nuvk_surface_create();
    if (result != NU_SUCCESS) return result;

    /* pick physical device */
    result = nuvk_physical_device_pick();
    if (result != NU_SUCCESS) return result;

    /* create device */
    result = nuvk_device_create();
    if (result != NU_SUCCESS) return result;

    /* create swap chain */
    result = nuvk_swapchain_create();
    if (result != NU_SUCCESS) return result;

    return result;
}
nu_result_t nuvk_renderer_terminate(void)
{
    /* destroy swapchain */
    nuvk_swapchain_destroy();

    /* destroy device */
    nuvk_device_destroy();

    /* destroy surface */
    nuvk_surface_destroy();

    /* destroy debug messenger */
#ifdef NU_DEBUG
    nuvk_debug_messenger_destroy();
#endif

    /* destroy instance */
    nuvk_instance_destroy();

    return NU_SUCCESS;
}
nu_result_t nuvk_renderer_render(void)
{
    return NU_SUCCESS;
}