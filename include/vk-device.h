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

struct VkDeviceCreateInfo create_device_info(void);

struct VkDeviceQueueCreateInfo create_device_queue_info(void);

uint32_t get_physical_device_count(struct VkInstance_T* vkInstance);

struct VkPhysicalDevice_T** create_physical_devices(
    struct VkInstance_T* vkInstance, uint32_t deviceCount
);

void destroy_physical_devices(struct VkPhysicalDevice_T** physicalDevices);

struct VkPhysicalDeviceProperties get_physical_device_properties(
    struct VkPhysicalDevice_T* selectedPhysicalDevice,
    uint32_t deviceCount
);

struct VkPhysicalDevice_T* select_physical_device(
    struct VkPhysicalDevice_T** physicalDevices,
    uint32_t deviceCount
);

uint32_t get_physical_device_queue_family_count(
    struct VkPhysicalDevice_T* selectedPhysicalDevice
);

struct VkQueueFamilyProperties* create_queue_family_properties(
    struct VkPhysicalDevice_T* selectedPhysicalDevice,
    uint32_t queueFamilyPropertyCount
);

void destroy_queue_family_properties(
    struct VkQueueFamilyProperties* queueFamilyProperties
);

uint32_t get_physical_device_queue_family(
    struct VkQueueFamilyProperties* queueFamilyProperties,
    uint32_t queueFamilyPropertyCount
);

/**
 * @brief Initializes the Vulkan device and selects the best available physical
 * device.
 *
 * @param vkInstance A pointer to a vulkan_instance_t structure.
 * @return A pointer to the created vulkan_device_t structure.
 */
vulkan_device_t* create_vulkan_device(vulkan_instance_t* vkInstance);

/**
 * @brief Cleans up and destroys the Vulkan device, including logical and
 * physical devices.
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
