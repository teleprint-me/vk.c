/**
 * Copyright © 2024 Austin Berrio
 *
 * @file include/vk-device.h
 *
 * @brief Vulkan Device API for managing Vulkan instances and devices.
 *
 * @note This API handles physical and logical device management, queue setup, 
 *       and other device-related operations.
 */

#ifndef VK_DEVICE
#define VK_DEVICE

#include "vk-instance.h"

#include <vulkan/vulkan.h>

// Alias for struct VkDevice_T* to distinguish logical devices
typedef struct VkDevice_T* VkLogicalDevice;

/**
 * @brief Structure representing a Vulkan device and related objects.
 */
typedef struct VulkanDevice {
    uint32_t physicalCount; ///< Number of physical devices
    VkPhysicalDevice physical; ///< Selected physical device
    VkPhysicalDeviceType type; ///< Type of the physical device
    VkPhysicalDeviceProperties properties; ///< Properties of the physical device
    VkLogicalDevice logical; ///< Logical device handle
    VkQueue queue; ///< Queue for the logical device
} vulkan_device_t;

// Functions for initializing and managing devices

/**
 * @brief Initializes the Vulkan device and selects the best available physical device.
 * 
 * @param vkInstance A Vulkan instance handle.
 * @return A pointer to the created vulkan_device_t structure.
 */
vulkan_device_t* create_vulkan_device(vulkan_instance_t* vkInstance);

/**
 * @brief Cleans up and destroys the Vulkan device, including logical and physical devices.
 * 
 * @param vkDevice A pointer to the vulkan_device_t structure to destroy.
 */
void destroy_vulkan_device(vulkan_device_t* vkDevice);

/**
 * @brief Creates a queue for the logical device.
 * 
 * @param vkDevice A pointer to the vulkan_device_t structure.
 * @param queueFamilyIndex Index of the queue family to create.
 */
void create_device_queue(vulkan_device_t* vkDevice, uint32_t queueFamilyIndex);

// Add other device-related functions as needed...

#endif // VK_DEVICE
