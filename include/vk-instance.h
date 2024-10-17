/**
 * Copyright © 2024 Austin Berrio
 *
 * @file include/vk-instance.h
 * 
 * @brief Create the VkInstance object with "sane" defaults.
 * 
 * @note Apply zero-initialization strategy to maintain a "sane" default implementation.
 */

#ifndef VK_INSTANCE
#define VK_INSTANCE

#include <vulkan/vulkan.h>

// @note Attempt to mitigate name conflicts
typedef struct VulkanInstance {
    struct VkApplicationInfo* applicationInfo; // Application-level info
    struct VkInstanceCreateInfo* instanceCreateInfo; // Vulkan instance creation info
    struct VkInstance_T* handle; // Actual Vulkan instance handle
} vulkan_instance_t;

// Create a struct VkApplicationInfo instance object
struct VkApplicationInfo create_vk_application_info(char* pApplicationName, char* pEngineName);

// Create a struct VkInstanceCreateInfo instance object
struct VkInstanceCreateInfo create_vk_instance_info(struct VkApplicationInfo* pApplicationInfo);

// Set extensions for an existing VkInstanceCreateInfo object
void set_instance_info_extensions(struct VkInstanceCreateInfo* pInstanceInfo, const char** extensions, uint32_t extensionCount);

// Set validation layers for an existing VkInstanceCreateInfo object
void set_instance_info_validation_layers(struct VkInstanceCreateInfo* pInstanceInfo, const char** layers, uint32_t layerCount);

// Create a struct VkInstance_T* (aka VkInstance) instance object
vulkan_instance_t* create_vulkan_instance(char* pApplicationName, char* pEngineName);

// Destroy an existing vulkan_instance_t object and free associated memory
void destroy_vulkan_instance(vulkan_instance_t* vkInstance);

#endif // VK_INSTANCE
