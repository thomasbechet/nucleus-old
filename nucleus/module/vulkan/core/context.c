#include <nucleus/module/vulkan/core/context.h>

#include <nucleus/module/vulkan/glfw/interface.h>

#define NUVK_LOGGER_NAME "[VULKAN] "

static nu_result_t nuvk_context_fill_required_instance_layers(nu_array_t layers)
{
    static uint32_t required_layer_count = 1;
    static const char *required_layers[] = {
        "VK_LAYER_KHRONOS_validation"
    };

    for (uint32_t i = 0; i < required_layer_count; i++) {
        nu_array_push(layers, &required_layers[i]);
    }

    return NU_SUCCESS;
}

static nu_result_t nuvk_context_fill_required_instance_extensions(nu_array_t extensions)
{
    static uint32_t required_extension_count = 0;
    static const char *required_extensions[] = {};

    for (uint32_t i = 0; i < required_extension_count; i++) {
        nu_array_push(extensions, &required_extensions[i]);
    }

    if (NUVK_CONTEXT_ENABLE_VALIDATION_LAYER) {
        static const char *debug_report_extension = VK_EXT_DEBUG_REPORT_EXTENSION_NAME;
        nu_array_push(extensions, &debug_report_extension);
    }

    uint32_t glfw_extension_count;
    const char **glfw_extensions;
    nuvk_glfw_get_required_instance_extensions(&glfw_extensions, &glfw_extension_count);

    for (uint32_t i = 0; i < glfw_extension_count; i++) {
        nu_array_push(extensions, &glfw_extensions[i]);
    }

    return NU_SUCCESS;
}
static nu_result_t nuvk_context_create_instance(nuvk_context_t *context, const VkAllocationCallbacks *allocator)
{
    nu_array_t required_layers;
    nu_array_allocate(sizeof(const char*), &required_layers);
    nuvk_context_fill_required_instance_layers(required_layers);

    nu_array_t required_extensions;
    nu_array_allocate(sizeof(const char*), &required_extensions);
    nuvk_context_fill_required_instance_extensions(required_extensions);

    VkApplicationInfo application_info;
    memset(&application_info, 0, sizeof(VkApplicationInfo));
    application_info.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    application_info.pApplicationName   = "VulkanApp"; 
    application_info.applicationVersion = 0; 
    application_info.pEngineName        = "VulkanEngine";
    application_info.engineVersion      = 0;
    application_info.apiVersion         = NUVK_CONTEXT_API_VERSION;

    VkInstanceCreateInfo instance_info;
    memset(&instance_info, 0, sizeof(VkInstanceCreateInfo));
    instance_info.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instance_info.flags                   = 0x0;
    instance_info.pApplicationInfo        = &application_info;
    instance_info.enabledLayerCount       = nu_array_get_size(required_layers);
    instance_info.ppEnabledLayerNames     = (const char *const *)nu_array_get_data_const(required_layers);
    instance_info.enabledExtensionCount   = nu_array_get_size(required_extensions);
    instance_info.ppEnabledExtensionNames = (const char *const *)nu_array_get_data_const(required_extensions);  

    VkResult error = vkCreateInstance(&instance_info, allocator, &context->instance);

    nu_array_free(required_layers);
    nu_array_free(required_extensions);

    if (error == VK_ERROR_EXTENSION_NOT_PRESENT) {
        nu_error(NUVK_LOGGER_NAME"Missing instance extension, failed to create instance.\n");
        return NU_FAILURE;
    } else if (error == VK_ERROR_LAYER_NOT_PRESENT) {
        nu_error(NUVK_LOGGER_NAME"Missing instance layer, failed to create instance.\n");
        return NU_FAILURE;
    } else if (error != 0) {
        nu_error(NUVK_LOGGER_NAME"Failed to create instance.\n");
        return NU_FAILURE;
    }

    return NU_SUCCESS;
}

static VkBool32 nuvk_context_report_callback_function(
    VkDebugReportFlagsEXT flags,
    VkDebugReportObjectTypeEXT object_type,
    uint64_t object,
    size_t location,
    int32_t message_code,
    const char *layer_prefix,
    const char *message,
    void *user
)
{
    if (flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT) {
        nu_info(NUVK_LOGGER_NAME"%s\n", message);
    } else if (flags & VK_DEBUG_REPORT_WARNING_BIT_EXT) {
        nu_warning(NUVK_LOGGER_NAME"%s\n", message);
    } else if (flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT) {
        nu_warning(NUVK_LOGGER_NAME"%s\n", message);
    } else if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT) {
        nu_error(NUVK_LOGGER_NAME"%s\n", message);
    } else if (flags & VK_DEBUG_REPORT_DEBUG_BIT_EXT) {
        nu_warning(NUVK_LOGGER_NAME"%s\n", message);
    } else {
        nu_info(NUVK_LOGGER_NAME"%s\n", message);
    }

    return VK_FALSE;
}
static nu_result_t nuvk_context_create_debug_report_callback(nuvk_context_t *context, const VkAllocationCallbacks *allocator)
{
    VkDebugReportCallbackCreateInfoEXT debug_info;
    memset(&debug_info, 0, sizeof(VkDebugReportCallbackCreateInfoEXT));
    debug_info.sType       = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
    debug_info.flags       = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
    debug_info.pfnCallback = nuvk_context_report_callback_function;
    debug_info.pUserData   = NULL;

    PFN_vkCreateDebugReportCallbackEXT create_debug_report_callback = VK_NULL_HANDLE;
    create_debug_report_callback = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(context->instance, "vkCreateDebugReportCallbackEXT");
    if (create_debug_report_callback == VK_NULL_HANDLE) {
        nu_error(NUVK_LOGGER_NAME"Failed to recover the vkCreateDebutReportCallbackEXT function.\n");
        return NU_FAILURE;
    }

    VkResult error = create_debug_report_callback(context->instance, &debug_info, allocator, &context->debug_report_callback);
    if (error != VK_SUCCESS) {
        nu_error(NUVK_LOGGER_NAME"Failed to create debug report callback.\n");
        return NU_FAILURE;
    }

    return NU_SUCCESS;
}

static nu_result_t nuvk_context_fill_required_device_extensions(nu_array_t extensions)
{
    uint32_t device_extension_count = 2;
    const char *device_extensions[] = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        "VK_KHR_maintenance1"
    };

    for (uint32_t i = 0; i < device_extension_count; i++) {
        nu_array_push(extensions, &device_extensions[i]);
    }

    return NU_SUCCESS;
}
static bool nuvk_context_is_physical_device_suitable(VkPhysicalDevice physical_device)
{
    uint32_t property_count;
    VkResult error = vkEnumerateDeviceExtensionProperties(physical_device, NULL, &property_count, NULL);
    if (error != VK_SUCCESS) {
        nu_warning(NUVK_LOGGER_NAME"Failed to enumerate device extension properties.\n");
        return false;
    }

    if (property_count == 0) {
        return false;
    }

    VkExtensionProperties *properties = (VkExtensionProperties*)nu_malloc(sizeof(VkExtensionProperties) * property_count);
    vkEnumerateDeviceExtensionProperties(physical_device, NULL, &property_count, properties);

    nu_array_t required_extensions;
    nu_array_allocate(sizeof(const char *), &required_extensions);
    nuvk_context_fill_required_device_extensions(required_extensions);
    
    bool missing_extension = false;
    for (uint32_t i = 0; i < nu_array_get_size(required_extensions); i++) {
        const char *required_extension = *(const char**)nu_array_get(required_extensions, i);
        bool extension_found = false;
        for (uint32_t j = 0; j < property_count; j++) {
            if (NU_MATCH(required_extension, properties[j].extensionName)) {
                extension_found = true;
                break;
            }
        }
        if (!extension_found) {
            missing_extension = true;
            break;
        }
    }

    nu_array_free(required_extensions);
    nu_free(properties);

    return !missing_extension;
}
static nu_result_t nuvk_context_pick_physical_device(nuvk_context_t *context)
{
    uint32_t physical_device_count;
    VkResult error = vkEnumeratePhysicalDevices(context->instance, &physical_device_count, NULL);
    if (error != VK_SUCCESS) {
        nu_error(NUVK_LOGGER_NAME"Failed to enumerate physical devices.\n");
        return NU_FAILURE;
    }

    if (physical_device_count == 0) {
        nu_error(NUVK_LOGGER_NAME"No physical device accessible.\n");
    }

    VkPhysicalDevice *physical_devices = (VkPhysicalDevice*)nu_malloc(sizeof(VkPhysicalDevice) * physical_device_count);
    vkEnumeratePhysicalDevices(context->instance, &physical_device_count, physical_devices);

    for (uint32_t i = 0; i < physical_device_count; i++) {
        if (nuvk_context_is_physical_device_suitable(physical_devices[i])) {
            context->physical_device = physical_devices[i];
            break;
        }
    }

    if (context->physical_device == VK_NULL_HANDLE) {
        nu_error(NUVK_LOGGER_NAME"Failed to find suitable physical device.\n");
        nu_free(physical_devices);
        return NU_FAILURE;
    }

    vkGetPhysicalDeviceProperties(context->physical_device, &context->physical_device_properties);

    nu_free(physical_devices);

    return NU_SUCCESS;
}

static nu_result_t nuvk_context_pick_queue_family_indices(nuvk_context_t *context)
{
    context->graphics_queue_family_index = INT32_MAX;
    context->present_queue_family_index  = INT32_MAX;

    uint32_t property_count;
    vkGetPhysicalDeviceQueueFamilyProperties(context->physical_device, &property_count, NULL);
    if (property_count == 0) {
        nu_error(NUVK_LOGGER_NAME"Failed to find queue family properties.\n");
        return NU_FAILURE;
    }

    VkQueueFamilyProperties *properties = (VkQueueFamilyProperties*)nu_malloc(sizeof(VkQueueFamilyProperties) * property_count);
    vkGetPhysicalDeviceQueueFamilyProperties(context->physical_device, &property_count, properties);

    for (uint32_t i = 0; i < property_count; i++) {
        if (properties[i].queueCount > 0 && (properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)) {
            context->graphics_queue_family_index = i;
        }

        VkBool32 supported;
        vkGetPhysicalDeviceSurfaceSupportKHR(context->physical_device, i, context->surface, &supported);
        if (properties[i].queueCount > 0 && supported == VK_TRUE) {
            context->present_queue_family_index = i;
        }

        if (context->graphics_queue_family_index != INT32_MAX && context->present_queue_family_index != INT32_MAX) {
            break;
        }
    }

    nu_free(properties);

    if (context->graphics_queue_family_index == INT32_MAX) {
        nu_error(NUVK_LOGGER_NAME"Failed to find graphics queue family index.\n");
        return NU_FAILURE;
    }

    if (context->present_queue_family_index == INT32_MAX) {
        nu_error(NUVK_LOGGER_NAME"Failed to find present queue family index.\n");
        return NU_FAILURE;
    }

    return NU_SUCCESS;
}

static nu_result_t nuvk_context_create_device(nuvk_context_t *context, const VkAllocationCallbacks *allocator)
{
    VkDeviceQueueCreateInfo queue_infos[2];
    memset(queue_infos, 0, sizeof(VkDeviceQueueCreateInfo) * 2);

    float priorities[] = {0.0f, 0.0f};
    uint32_t queue_info_count = 0;

    if (context->graphics_queue_family_index == context->present_queue_family_index) {
        queue_infos[0].sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queue_infos[0].flags            = 0x0;
        queue_infos[0].queueFamilyIndex = context->graphics_queue_family_index;
        queue_infos[0].queueCount       = 2;
        queue_infos[0].pQueuePriorities = priorities;
        
        queue_info_count = 1;
    } else {
        queue_infos[0].sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queue_infos[0].flags            = 0x0;
        queue_infos[0].queueFamilyIndex = context->graphics_queue_family_index;
        queue_infos[0].queueCount       = 1;
        queue_infos[0].pQueuePriorities = priorities;

        queue_infos[1].sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queue_infos[1].flags            = 0x0;
        queue_infos[1].queueFamilyIndex = context->present_queue_family_index;
        queue_infos[1].queueCount       = 1;
        queue_infos[1].pQueuePriorities = priorities;
    
        queue_info_count = 2;
    }

    nu_array_t extensions;
    nu_array_allocate(sizeof(const char *), &extensions);
    nuvk_context_fill_required_device_extensions(extensions);

    VkPhysicalDeviceFeatures features;
    memset(&features, 0, sizeof(VkPhysicalDeviceFeatures));
    features.shaderClipDistance = VK_TRUE;

    VkDeviceCreateInfo device_info;
    memset(&device_info, 0, sizeof(VkDeviceCreateInfo));
    device_info.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    device_info.flags                   = 0x0;
    device_info.queueCreateInfoCount    = queue_info_count;
    device_info.pQueueCreateInfos       = queue_infos;
    device_info.enabledLayerCount       = 0;
    device_info.ppEnabledLayerNames     = NULL;
    device_info.enabledExtensionCount   = nu_array_get_size(extensions);
    device_info.ppEnabledExtensionNames = (const char *const *)nu_array_get_data_const(extensions);
    device_info.pEnabledFeatures        = &features;

    VkResult error = vkCreateDevice(context->physical_device, &device_info, allocator, &context->device);

    nu_array_free(extensions);

    if (error != VK_SUCCESS) {
        nu_error(NUVK_LOGGER_NAME"Failed to create device.\n");
        return NU_FAILURE;
    }

    return NU_SUCCESS;
}

static nu_result_t nuvk_context_pick_queues(nuvk_context_t *context)
{
    if (context->graphics_queue_family_index == context->present_queue_family_index) {
        vkGetDeviceQueue(context->device, context->graphics_queue_family_index, 0, &context->graphics_queue);
        vkGetDeviceQueue(context->device, context->present_queue_family_index, 1, &context->present_queue);
    } else {
        vkGetDeviceQueue(context->device, context->graphics_queue_family_index, 0, &context->graphics_queue);
        vkGetDeviceQueue(context->device, context->present_queue_family_index, 0, &context->present_queue);
    }

    return NU_SUCCESS;
}

nu_result_t nuvk_context_initialize(nuvk_context_t *context, const VkAllocationCallbacks *allocator)
{
    if (nuvk_context_create_instance(context, allocator) != NU_SUCCESS) {
        return NU_FAILURE;
    }

    if (nuvk_glfw_create_window_surface(context->instance, &context->surface, allocator) != NU_SUCCESS) {
        return NU_FAILURE;
    }

    if (NUVK_CONTEXT_ENABLE_VALIDATION_LAYER) {
        if (nuvk_context_create_debug_report_callback(context, allocator) != NU_SUCCESS) {
            return NU_FAILURE;
        }
    }

    if (nuvk_context_pick_physical_device(context) != NU_SUCCESS) {
        return NU_FAILURE;
    }

    if (nuvk_context_pick_queue_family_indices(context) != NU_SUCCESS) {
        return NU_FAILURE;
    }

    if (nuvk_context_create_device(context, allocator) != NU_SUCCESS) {
        return NU_FAILURE;
    }

    if (nuvk_context_pick_queues(context) != NU_SUCCESS) {
        return NU_FAILURE;
    }

    return NU_SUCCESS;
}
nu_result_t nuvk_context_terminate(nuvk_context_t *context, const VkAllocationCallbacks *allocator)
{
    vkDeviceWaitIdle(context->device);

    vkDestroyDevice(context->device, allocator);
    if (NUVK_CONTEXT_ENABLE_VALIDATION_LAYER) {
        PFN_vkDestroyDebugReportCallbackEXT destroy_debug_report_callback = VK_NULL_HANDLE;
        destroy_debug_report_callback = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(context->instance, "vkDestroyDebugReportCallbackEXT");
        if (destroy_debug_report_callback == VK_NULL_HANDLE) {
            nu_warning(NUVK_LOGGER_NAME"Failed to recover the vkCreateDebutReportCallbackEXT function.\n");
        } else {
            destroy_debug_report_callback(context->instance, context->debug_report_callback, allocator);
        }
    }
    vkDestroySurfaceKHR(context->instance, context->surface, allocator);
    vkDestroyInstance(context->instance, allocator);

    return NU_SUCCESS;
}