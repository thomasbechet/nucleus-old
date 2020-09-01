#include "swapchain.h"

#include "../common/logger.h"
#include "surface.h"
#include "device.h"
#include "physical_device.h"

typedef struct {
    VkSurfaceCapabilitiesKHR capabilities;
    uint32_t format_count;
    VkSurfaceFormatKHR *formats;
    uint32_t present_mode_count;
    VkPresentModeKHR *present_modes;
} nuvk_swapchain_support_details_t;

static nu_result_t swapchain_support_details_create(
    nuvk_swapchain_support_details_t *details,
    VkPhysicalDevice physical_device,
    VkSurfaceKHR surface
)
{
    /* get physical device capatibilities */
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device, surface, &details->capabilities);

    /* get surface formats */
    vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &details->format_count, NULL);
    if (details->format_count > 0) {
        details->formats = (VkSurfaceFormatKHR*)nu_malloc(sizeof(VkSurfaceFormatKHR) * details->format_count);
        vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &details->format_count, details->formats);
    }

    /* get surface present modes */
    vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &details->present_mode_count, NULL);
    if (details->present_mode_count > 0) {
        details->present_modes = (VkPresentModeKHR*)nu_malloc(sizeof(VkPresentModeKHR) * details->present_mode_count);
        vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &details->present_mode_count, details->present_modes);
    }

    return NU_SUCCESS;
}
static nu_result_t swapchain_support_details_destroy(nuvk_swapchain_support_details_t *details)
{
    /* free formats */
    if (details->format_count > 0) {
        nu_free(details->formats);
        details->format_count = 0;
    }

    /* free present modes */
    if (details->present_mode_count > 0) {
        nu_free(details->present_modes);
        details->present_mode_count = 0;
    }

    return NU_SUCCESS;
}

static VkSurfaceFormatKHR choose_swap_surface_format(const nuvk_swapchain_support_details_t *details)
{
    for (uint32_t i = 0; i < details->format_count; i++) {
        if (details->formats[i].format == VK_FORMAT_B8G8R8A8_SRGB && details->formats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return details->formats[i];
        }
    }

    return details->formats[0];
}
static VkPresentModeKHR choose_swap_present_format(const nuvk_swapchain_support_details_t *details)
{
    return VK_PRESENT_MODE_FIFO_KHR;

    for (uint32_t i = 0; i < details->present_mode_count; i++) {
        if (details->present_modes[i] == VK_PRESENT_MODE_MAILBOX_KHR) {
            return details->present_modes[i];
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}
static VkExtent2D choose_swap_extent(const nuvk_swapchain_support_details_t *details)
{
    if (details->capabilities.currentExtent.width != UINT32_MAX) {
        return details->capabilities.currentExtent;
    } else {
        uint32_t width, height;
        nu_window_get_size(&width, &height);

        VkExtent2D actualExtent = {width, height};
        actualExtent.width = NU_MAX(details->capabilities.minImageExtent.width, NU_MIN(details->capabilities.maxImageExtent.width, actualExtent.width));
        actualExtent.height = NU_MAX(details->capabilities.minImageExtent.height, NU_MIN(details->capabilities.maxImageExtent.height, actualExtent.height));
        return actualExtent;
    }
}

static nu_result_t create_images(nuvk_swapchain_t *swapchain, VkDevice device)
{
    /* recover images */
    uint32_t image_count;
    vkGetSwapchainImagesKHR(device, swapchain->handle, &image_count, NULL);
    VkImage *images = (VkImage*)nu_malloc(sizeof(VkImage) * image_count);
    vkGetSwapchainImagesKHR(device, swapchain->handle, &image_count, images);

    /* create image views */
    swapchain->image_count = image_count;
    swapchain->images = (VkImageView*)nu_malloc(sizeof(VkImageView) * swapchain->image_count);
    for (uint32_t i = 0; i < swapchain->image_count; i++) {
        VkImageViewCreateInfo create_info;
        memset(&create_info, 0, sizeof(VkImageViewCreateInfo));
        create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        create_info.image = images[i];
        create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
        create_info.format = swapchain->format;
        create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        create_info.subresourceRange.baseMipLevel = 0;
        create_info.subresourceRange.levelCount = 1;
        create_info.subresourceRange.baseArrayLayer = 0;
        create_info.subresourceRange.layerCount = 1;

        if (vkCreateImageView(device, &create_info, NULL, &swapchain->images[i]) != VK_SUCCESS) {
            nu_warning(NUVK_LOGGER_NAME"Failed to create image views.\n");
        }
    }

    nu_free(images);

    return NU_SUCCESS;
}
static nu_result_t destroy_images(nuvk_swapchain_t swapchain, VkDevice device)
{
    /* destroy image views */
    for (uint32_t i = 0; i < swapchain.image_count; i++) {
        vkDestroyImageView(device, swapchain.images[i], NULL);
    }
    nu_free(swapchain.images);

    return NU_SUCCESS;
}

nu_result_t nuvk_swapchain_create(nuvk_swapchain_t *swapchain, VkSurfaceKHR surface, VkPhysicalDevice physical_device, VkDevice device)
{
    nu_result_t result;
    result = NU_SUCCESS;

    /* retrieve details infos */
    nuvk_swapchain_support_details_t details;
    result = swapchain_support_details_create(&details, physical_device, surface);
    if (result != NU_SUCCESS) return result;

    /* get format */
    VkSurfaceFormatKHR surface_format = choose_swap_surface_format(&details);
    /* get present mode */
    VkPresentModeKHR present_mode = choose_swap_present_format(&details);
    /* get extent */
    VkExtent2D extent = choose_swap_extent(&details);
    /* get image count */
    uint32_t image_count = details.capabilities.minImageCount + 1;
    uint32_t max_image_count = details.capabilities.maxImageCount;
    if (max_image_count > 0 && image_count > max_image_count) {
        image_count = max_image_count;
    }

    /* save info */
    swapchain->format = surface_format.format;
    swapchain->extent = extent;
    swapchain->present_mode = present_mode;
    swapchain->image_count = image_count;

    /* create swapchain */
    VkSwapchainCreateInfoKHR create_info;
    memset(&create_info, 0, sizeof(VkSwapchainCreateInfoKHR));
    create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    create_info.surface = surface;
    create_info.minImageCount = swapchain->image_count;
    create_info.imageFormat = swapchain->format;
    create_info.imageColorSpace = surface_format.colorSpace;
    create_info.imageExtent = swapchain->extent;
    create_info.imageArrayLayers = 1;
    create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    nuvk_queue_family_indices_t indices;
    nuvk_physical_device_get_queue_family_indices(&indices, surface, physical_device);
    uint32_t queue_family_indices[] = {indices.graphics_family, indices.presentation_family};
    if (indices.graphics_family != indices.presentation_family) {
        create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        create_info.queueFamilyIndexCount = 2;
        create_info.pQueueFamilyIndices = queue_family_indices;
    } else {
        create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        create_info.queueFamilyIndexCount = 0;
        create_info.pQueueFamilyIndices = NULL;
    }

    create_info.preTransform = details.capabilities.currentTransform;
    create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    create_info.presentMode = swapchain->present_mode;
    create_info.clipped = VK_TRUE;
    create_info.oldSwapchain = VK_NULL_HANDLE;

    swapchain_support_details_destroy(&details);

    if (vkCreateSwapchainKHR(device, &create_info, NULL, &swapchain->handle) != VK_SUCCESS) {
        nu_warning(NUVK_LOGGER_NAME"Failed to create swapchain.\n");
        return NU_FAILURE;
    }

    /* create image views */
    create_images(swapchain, device);

    return result;
}
nu_result_t nuvk_swapchain_destroy(nuvk_swapchain_t swapchain, VkDevice device)
{
    destroy_images(swapchain, device);
    vkDestroySwapchainKHR(device, swapchain.handle, NULL);

    return NU_SUCCESS;
}

bool nuvk_swapchain_is_physical_device_suitable(VkPhysicalDevice physical_device, VkSurfaceKHR surface)
{
    nuvk_swapchain_support_details_t details;
    swapchain_support_details_create(&details, physical_device, surface);
    bool suitable = details.format_count > 0 && details.present_mode_count > 0;
    swapchain_support_details_destroy(&details);

    return suitable;
}