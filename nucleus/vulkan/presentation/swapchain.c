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

typedef struct {
    VkFormat format;
    VkExtent2D extent;
    VkPresentModeKHR present_mode;

    uint32_t image_count;
    VkImage *images;
    VkImageView *image_views;

    VkSwapchainKHR swapchain;
} nuvk_swapchain_data_t;

static nuvk_swapchain_data_t _data;

static nu_result_t nuvk_swapchain_support_details_create(
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
static nu_result_t nuvk_swapchain_support_details_destroy(nuvk_swapchain_support_details_t *details)
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

static VkSurfaceFormatKHR nuvk_choose_swap_surface_format(const nuvk_swapchain_support_details_t *details)
{
    for (uint32_t i = 0; i < details->format_count; i++) {
        if (details->formats[i].format == VK_FORMAT_B8G8R8A8_SRGB && details->formats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return details->formats[i];
        }
    }

    return details->formats[0];
}
static VkPresentModeKHR nuvk_choose_swap_present_format(const nuvk_swapchain_support_details_t *details)
{
    for (uint32_t i = 0; i < details->present_mode_count; i++) {
        if (details->present_modes[i] == VK_PRESENT_MODE_MAILBOX_KHR) {
            return details->present_modes[i];
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}
static VkExtent2D nuvk_choose_swap_extent(const nuvk_swapchain_support_details_t *details)
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

static nu_result_t nuvk_create_images(void)
{
    /* recover images */
    vkGetSwapchainImagesKHR(nuvk_device_get_handle(), _data.swapchain, 
        &_data.image_count, NULL);
    _data.images = (VkImage*)nu_malloc(sizeof(VkImage) * _data.image_count);
    vkGetSwapchainImagesKHR(nuvk_device_get_handle(), _data.swapchain, 
        &_data.image_count, _data.images);

    /* create image views */
    _data.image_views = (VkImageView*)nu_malloc(sizeof(VkImageView) * _data.image_count);
    for (uint32_t i = 0; i < _data.image_count; i++) {
        VkImageViewCreateInfo create_info;
        memset(&create_info, 0, sizeof(VkImageViewCreateInfo));
        create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        create_info.image = _data.images[i];
        create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
        create_info.format = _data.format;
        create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        create_info.subresourceRange.baseMipLevel = 0;
        create_info.subresourceRange.levelCount = 1;
        create_info.subresourceRange.baseArrayLayer = 0;
        create_info.subresourceRange.layerCount = 1;

        if (vkCreateImageView(nuvk_device_get_handle(), &create_info, NULL, &_data.image_views[i]) != VK_SUCCESS) {
            nu_warning(NUVK_LOGGER_VULKAN" Failed to create image views.\n");
        }
    }

    return NU_SUCCESS;
}
static nu_result_t nuvk_destroy_images(void)
{
    /* destroy image views */
    for (uint32_t i = 0; i < _data.image_count; i++) {
        vkDestroyImageView(nuvk_device_get_handle(), _data.image_views[i], NULL);
    }
    nu_free(_data.image_views);

    /* free images */
    nu_free(_data.images);

    return NU_SUCCESS;
}

nu_result_t nuvk_swapchain_create(void)
{
    nu_result_t result;
    result = NU_SUCCESS;

    /* retrieve details infos */
    nuvk_swapchain_support_details_t details;
    result = nuvk_swapchain_support_details_create(&details, nuvk_physical_device_get_handle(), nuvk_surface_get_handle());
    if (result != NU_SUCCESS) return result;

    VkSurfaceFormatKHR surface_format = nuvk_choose_swap_surface_format(&details);
    VkPresentModeKHR present_mode = nuvk_choose_swap_present_format(&details);
    VkExtent2D extent = nuvk_choose_swap_extent(&details);

    uint32_t image_count = details.capabilities.minImageCount + 1;
    uint32_t max_image_count = details.capabilities.maxImageCount;
    if (max_image_count > 0 && image_count > max_image_count) {
        image_count = max_image_count;
    }

    _data.image_count = image_count;
    _data.format = surface_format.format;
    _data.extent = extent;
    _data.present_mode = present_mode;

    /* create swapchain */
    VkSwapchainCreateInfoKHR create_info;
    memset(&create_info, 0, sizeof(VkSwapchainCreateInfoKHR));
    create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    create_info.surface = nuvk_surface_get_handle();
    create_info.minImageCount = _data.image_count;
    create_info.imageFormat = _data.format;
    create_info.imageColorSpace = surface_format.colorSpace;
    create_info.imageExtent = _data.extent;
    create_info.imageArrayLayers = 1;
    create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    nuvk_queue_family_indices_t indices = nuvk_physical_device_get_queue_family_indices();
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
    create_info.presentMode = _data.present_mode;
    create_info.clipped = VK_TRUE;
    create_info.oldSwapchain = VK_NULL_HANDLE;

    nuvk_swapchain_support_details_destroy(&details);

    if (vkCreateSwapchainKHR(nuvk_device_get_handle(), &create_info, NULL, &_data.swapchain) != VK_SUCCESS) {
        nu_warning(NUVK_LOGGER_VULKAN" Failed to create swapchain.\n");
        return NU_FAILURE;
    }

    _data.format = surface_format.format;
    _data.extent = extent;

    /* create image */
    nuvk_create_images();

    return result;
}
nu_result_t nuvk_swapchain_destroy(void)
{
    /* destroy image views */
    nuvk_destroy_images();

    vkDestroySwapchainKHR(nuvk_device_get_handle(), _data.swapchain, NULL);

    return NU_SUCCESS;
}

bool nuvk_swapchain_is_physical_device_suitable(VkPhysicalDevice physical_device)
{
    nuvk_swapchain_support_details_t details;
    nuvk_swapchain_support_details_create(&details, physical_device, nuvk_surface_get_handle());

    bool suitable = details.format_count > 0 && details.present_mode_count > 0;

    nuvk_swapchain_support_details_destroy(&details);

    return suitable;
}