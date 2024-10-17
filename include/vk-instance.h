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

// Create a struct VkApplicationInfo instance object
struct VkApplicationInfo create_vk_application_info(char* pApplicationName, char* pEngineName);

// Create a struct VkInstanceCreateInfo instance object
struct VkInstanceCreateInfo create_vk_instance_info(struct VkApplicationInfo* pApplicationInfo);

// Set extensions for an existing VkInstanceCreateInfo object
void set_instance_info_extensions(struct VkInstanceCreateInfo* pInstanceInfo, const char** extensions, uint32_t extensionCount);

// Set validation layers for an existing VkInstanceCreateInfo object
void set_instance_info_validation_layers(struct VkInstanceCreateInfo* pInstanceInfo, const char** layers, uint32_t layerCount);

// Create a struct VkInstance_T* (aka VkInstance) instance object
struct VkInstance_T* create_vk_instance(struct VkInstanceCreateInfo* pInstanceInfo);

#endif // VK_INSTANCE
