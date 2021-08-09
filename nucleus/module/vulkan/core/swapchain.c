#include <nucleus/module/vulkan/core/swapchain.h>

static nu_result_t nuvk_swapchain_create(nuvk_swapchain_t *swapchain, const nuvk_context_t *context, uint32_t width, uint32_t height, VkSwapchainKHR old_swapchain)
{
    /* get surface capatibilities */
    VkSurfaceCapabilitiesKHR capatibilities;
    if (vkGetPhysicalDeviceSurfaceCapabilitiesKHR(context->physical_device, context->surface, &capatibilities) != VK_SUCCESS) {
        nu_error(NUVK_LOGGER_NAME"Failed to get physical device surface capatibilities.\n");
        return NU_FAILURE;
    }

    /* get present modes */
    uint32_t present_mode_count;
    if (vkGetPhysicalDeviceSurfacePresentModesKHR(context->physical_device, context->surface, &present_mode_count, NULL) != VK_SUCCESS) {
        nu_error(NUVK_LOGGER_NAME"Failed to get physical device surface present modes.\n");
        return NU_FAILURE;
    }

    VkPresentModeKHR *present_modes = (VkPresentModeKHR*)nu_malloc(sizeof(VkPresentModeKHR) * present_mode_count);
    vkGetPhysicalDeviceSurfacePresentModesKHR(context->physical_device, context->surface, &present_mode_count, present_modes);

    VkPresentModeKHR present_mode = VK_PRESENT_MODE_FIFO_KHR;
    for (uint32_t i = 0; i < present_mode_count; i++) {
        if (present_modes[i] == VK_PRESENT_MODE_MAILBOX_KHR) {
            present_mode = VK_PRESENT_MODE_MAILBOX_KHR;
            break;
        }
        if (present_modes[i] == VK_PRESENT_MODE_IMMEDIATE_KHR) {
            present_mode = VK_PRESENT_MODE_IMMEDIATE_KHR;
            break;
        }
    }
    present_mode = VK_PRESENT_MODE_FIFO_KHR;

    nu_free(present_modes);

    /* get surface format and color space */
    uint32_t surface_format_count;
    if (vkGetPhysicalDeviceSurfaceFormatsKHR(context->physical_device, context->surface, &surface_format_count, NULL) != VK_SUCCESS) {
        nu_error(NUVK_LOGGER_NAME"Failed to get physical device surface formats.\n");
        return NU_FAILURE;
    }

    VkSurfaceFormatKHR *surface_formats = (VkSurfaceFormatKHR*)nu_malloc(sizeof(VkSurfaceFormatKHR) * surface_format_count);
    vkGetPhysicalDeviceSurfaceFormatsKHR(context->physical_device, context->surface, &surface_format_count, surface_formats);

    if (surface_format_count == 1 && surface_formats[0].format == VK_FORMAT_UNDEFINED) {
        swapchain->surface_color_format = VK_FORMAT_B8G8R8A8_UNORM;
        swapchain->surface_color_space  = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
    } else {
        swapchain->surface_color_format = surface_formats[0].format;
        swapchain->surface_color_space  = surface_formats[0].colorSpace;
    }

    nu_free(surface_formats);

    /* find number of images */
    uint32_t desired_image_count = capatibilities.minImageCount + 1;
    if (capatibilities.maxImageCount > 0 && desired_image_count > capatibilities.maxImageCount) {
        desired_image_count = capatibilities.maxImageCount;
    }

    /* find extent */
    VkExtent2D extent = capatibilities.currentExtent;
    if (extent.width != INT32_MAX) {
        swapchain->extent = extent;
    } else {
        VkExtent2D min = capatibilities.minImageExtent;
        VkExtent2D max = capatibilities.maxImageExtent;
        width = NU_MAX(width, min.width);
        width = NU_MIN(width, max.width);
        height = NU_MAX(height, min.height);
        height = NU_MIN(height, max.height);
        swapchain->extent.width  = width;
        swapchain->extent.height = height;
    }

    /* find transform */
    VkSurfaceTransformFlagBitsKHR pre_transform;
    if (capatibilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR) {
        pre_transform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    } else {
        pre_transform = capatibilities.currentTransform;
    }

    /* create swapchain */
    VkSwapchainCreateInfoKHR info;
    memset(&info, 0, sizeof(VkSwapchainCreateInfoKHR));
    info.sType            = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    info.surface          = context->surface;
    info.minImageCount    = desired_image_count;
    info.imageFormat      = swapchain->surface_color_format;
    info.imageColorSpace  = swapchain->surface_color_space;
    info.imageExtent      = swapchain->extent;
    info.imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    info.preTransform     = pre_transform;
    info.imageArrayLayers = 1;
    info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    info.presentMode      = present_mode;
    info.oldSwapchain     = old_swapchain;
    info.clipped          = VK_TRUE;
    info.compositeAlpha   = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

    if (vkCreateSwapchainKHR(context->device, &info, &context->allocator, &swapchain->swapchain) != VK_SUCCESS) {
        nu_error(NUVK_LOGGER_NAME"Failed to create swapchain.\n");
        return NU_FAILURE;
    }

    /* destroy previous swapchain and images */
    if (old_swapchain != VK_NULL_HANDLE) {
        vkDestroySwapchainKHR(context->device, old_swapchain, &context->allocator);
        for (uint32_t i = 0; i < swapchain->image_count; i++) {
            vkDestroyImageView(context->device, swapchain->image_views[i], &context->allocator);
        }
        nu_free(swapchain->images);
        nu_free(swapchain->image_views);
    }

    /* recover images */
    if (vkGetSwapchainImagesKHR(context->device, swapchain->swapchain, &swapchain->image_count, NULL) != VK_SUCCESS) {
        nu_error(NUVK_LOGGER_NAME"Failed to get swapchain images.\n");
        return NU_FAILURE;
    }

    swapchain->images = (VkImage*)nu_malloc(sizeof(VkImage) * swapchain->image_count);
    vkGetSwapchainImagesKHR(context->device, swapchain->swapchain, &swapchain->image_count, swapchain->images);

    /* create image views */
    swapchain->image_views = (VkImageView*)nu_malloc(sizeof(VkImageView) * swapchain->image_count);

    for (uint32_t i = 0; i < swapchain->image_count; i++) {
        VkImageViewCreateInfo view_info;
        memset(&view_info, 0, sizeof(VkImageViewCreateInfo));
        view_info.sType                       = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        view_info.format                      = swapchain->surface_color_format;
        view_info.viewType                    = VK_IMAGE_VIEW_TYPE_2D;
        view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        view_info.subresourceRange.layerCount = 1;
        view_info.subresourceRange.levelCount = 1;
        view_info.image = swapchain->images[i];
        if (vkCreateImageView(context->device, &view_info, &context->allocator, &swapchain->image_views[i]) != VK_SUCCESS) {
            nu_error(NUVK_LOGGER_NAME"Failed to create image view.\n");
            return NU_FAILURE;
        }
    }

    return NU_SUCCESS;
}

nu_result_t nuvk_swapchain_initialize(nuvk_swapchain_t *swapchain, const nuvk_context_t *context, uint32_t width, uint32_t height)
{
    return nuvk_swapchain_create(swapchain, context, width, height, VK_NULL_HANDLE);
}
nu_result_t nuvk_swapchain_terminate(nuvk_swapchain_t *swapchain, const nuvk_context_t *context)
{
    if (swapchain->swapchain != VK_NULL_HANDLE) {
        vkDestroySwapchainKHR(context->device, swapchain->swapchain, &context->allocator);
        for (uint32_t i = 0; i < swapchain->image_count; i++) {
            vkDestroyImageView(context->device, swapchain->image_views[i], &context->allocator);
        }
        nu_free(swapchain->images);
        nu_free(swapchain->image_views);
    }

    return NU_SUCCESS;
}
nu_result_t nuvk_swapchain_recreate(nuvk_swapchain_t *swapchain, const nuvk_context_t *context, uint32_t width, uint32_t height)
{
    return nuvk_swapchain_create(swapchain, context, width, height, swapchain->swapchain);
}