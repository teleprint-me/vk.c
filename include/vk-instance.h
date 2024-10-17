/**
 * @file include/vk-instance.h
 * 
 * @note Apply zero-initialization strategy to maintain a "sane" default implementation.
 */

#ifndef VK_INSTANCE
#define VK_INSTANCE

#include <vulkan/vulkan.h>

struct VkApplicationInfo create_vk_application_info(char* pApplicationName, char* pEngineName);

struct VkInstanceCreateInfo create_vk_instance_info(struct VkApplicationInfo* pApplicationInfo);

// dedicated function for adding extensions to an existing VkInstanceCreateInfo object instance

// dedicated function for adding validation layers to an existing VkInstanceInfo object instance

struct VkInstance_T* create_vk_instance(struct VkInstanceCreateInfo* pInstanceInfo);

#endif // VK_INSTANCE
