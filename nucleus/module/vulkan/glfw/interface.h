#ifndef NUVK_GLFW_INTERFACE_H
#define NUVK_GLFW_INTERFACE_H

#include <nucleus/nucleus.h>
#include <vulkan/vulkan.h>

nu_result_t nuvk_glfw_load_interface(void);
nu_result_t nuvk_glfw_create_window_surface(VkInstance instance, VkSurfaceKHR *surface);
nu_result_t nuvk_glfw_get_required_instance_extensions(const char ***required_extensions, uint32_t *count);

#endif