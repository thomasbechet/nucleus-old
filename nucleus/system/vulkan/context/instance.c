#include "instance.h"

#include "../common/logger.h"
#include "../glfw/interface.h"

static bool nuvk_check_instance_extensions_support(
    const char **extensions,
    size_t extension_count
)
{
    uint32_t extension_property_count;
    VkExtensionProperties *extension_properties;

    vkEnumerateInstanceExtensionProperties(NULL, &extension_property_count, NULL);
    extension_properties = (VkExtensionProperties*)nu_malloc(sizeof(VkExtensionProperties) * extension_property_count);
    vkEnumerateInstanceExtensionProperties(NULL, &extension_property_count, extension_properties);

    bool all_extension_found = true;
    for (uint32_t i = 0; i < extension_count && all_extension_found; i++) {
        all_extension_found = false;
        for (uint32_t j = 0; j < extension_property_count; j++) {
            if (strcmp(extensions[i], extension_properties[j].extensionName) == 0) {
                all_extension_found = true;
                break;
            }
        }
        if (!all_extension_found) {
            nu_warning(NUVK_LOGGER_NAME"Instance extension '%s' not supported.\n", extensions[i]);
        }
    }

    nu_free(extension_properties);
    return all_extension_found;
}
static bool nuvk_check_instance_layers_support(
    const char **layers,
    size_t layer_count
)
{
    uint32_t layer_property_count;
    VkLayerProperties *layer_properties;

    vkEnumerateInstanceLayerProperties(&layer_property_count, NULL);
    layer_properties = (VkLayerProperties*)nu_malloc(sizeof(VkLayerProperties) * layer_property_count);
    vkEnumerateInstanceLayerProperties(&layer_property_count, layer_properties);

    bool all_layer_found = true;
    for (uint32_t i = 0; i < layer_count && all_layer_found; i++) {
        all_layer_found = false;
        for (uint32_t j = 0; j < layer_property_count; j++) {
            if (strcmp(layers[i], layer_properties[j].layerName) == 0) {
                all_layer_found = true;
                break;
            }
        }
        if (!all_layer_found) {
            nu_warning(NUVK_LOGGER_NAME"Instance layer '%s' not supported.\n", layers[i]);
        }
    }

    nu_free(layer_properties);
    return all_layer_found;
}

nu_result_t nuvk_instance_create(VkInstance *instance)
{
    /* Get required instance extensions (from glfw) */
    const char **required_instance_extensions;
    uint32_t required_instance_extension_count;
    if (nuvk_glfw_get_required_instance_extensions(&required_instance_extensions, &required_instance_extension_count) != NU_SUCCESS) {
        return NU_FAILURE;
    }

    /* Application info */
    VkApplicationInfo appInfo;
    memset(&appInfo, 0, sizeof(VkApplicationInfo));
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello Triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 2, 0);
    appInfo.pEngineName = "My Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 2, 0);
    appInfo.apiVersion = VK_API_VERSION_1_2;

    /* Additional extensions */
#ifdef NU_DEBUG
    uint32_t additional_extension_count = 1;
    const char *additional_extensions[] = {
        VK_EXT_DEBUG_UTILS_EXTENSION_NAME
    };
#else
    uint32_t additional_extension_count = 0;
    const char *additional_extensions[] = {};
#endif
    
    /* Additional layers */
#ifdef NU_DEBUG
    uint32_t additional_layer_count = 1;
    const char *additional_layers[] = {
        "VK_LAYER_KHRONOS_validation"
    };
#else
    uint32_t additional_layer_count = 0;
    const char *additional_layers[] = {};
#endif
    
    /* Instance Extensions */
    const char **instance_extensions;
    uint32_t instance_extension_count;

    instance_extension_count = required_instance_extension_count + additional_extension_count;
    instance_extensions = (const char**)nu_malloc(sizeof(const char*) * instance_extension_count);
    for (uint32_t i = 0; i < required_instance_extension_count; i++)
        instance_extensions[i] = required_instance_extensions[i];
    for (uint32_t i = 0; i < additional_extension_count; i++)
        instance_extensions[required_instance_extension_count + i] = additional_extensions[i];

    /* Layers */
    uint32_t instance_layer_count;
    const char **instance_layers;

    instance_layer_count = additional_layer_count;
    instance_layers = (const char**)nu_malloc(sizeof(const char*) * instance_layer_count);
    for (uint32_t i = 0; i < additional_layer_count; i++)
        instance_layers[i] = additional_layers[i];

    /* Check extensions */
    if (!nuvk_check_instance_extensions_support(instance_extensions, instance_extension_count)) {
        nu_free(instance_extensions);
        nu_free(instance_layers);
        return NU_FAILURE;   
    }

    /* Check layers */
    if (!nuvk_check_instance_layers_support(instance_layers, instance_layer_count)) {
        nu_free(instance_extensions);
        nu_free(instance_layers);
        return NU_FAILURE; 
    }

    /* Create instance */
    VkInstanceCreateInfo create_info;
    memset(&create_info, 0, sizeof(VkInstanceCreateInfo));
    create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    create_info.pApplicationInfo = &appInfo;
    create_info.enabledExtensionCount = instance_extension_count;
    create_info.ppEnabledExtensionNames = instance_extensions;
    create_info.enabledLayerCount = instance_layer_count;
    create_info.ppEnabledLayerNames = instance_layers;

    VkResult result;
    result = vkCreateInstance(&create_info, NULL, instance);
    if (result != VK_SUCCESS) {
        nu_warning(NUVK_LOGGER_NAME"Failed to create instance.\n");
        nu_free(instance_extensions);
        nu_free(instance_layers);
        return NU_FAILURE;
    }

    /* free resources */
    nu_free(instance_extensions);
    nu_free(instance_layers);

    return result == VK_SUCCESS ? NU_SUCCESS : NU_FAILURE;
}
nu_result_t nuvk_instance_destroy(VkInstance instance)
{
    vkDestroyInstance(instance, NULL);

    return NU_SUCCESS;
}