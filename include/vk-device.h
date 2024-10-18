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

struct VkDeviceCreateInfo* create_device_info(void);

struct VkDeviceQueueCreateInfo* create_device_queue_info(void);

uint32_t enumerate_physical_device_count(struct VkInstance_T* vkInstance);

struct VkPhysicalDevice_T** enumerate_physical_devices(
    struct VkInstance_T* vkInstance, uint32_t deviceCount
);

void free_physical_device_handles(struct VkPhysicalDevice_T** physicalDevices);

struct VkPhysicalDeviceProperties get_physical_device_properties(
    struct VkPhysicalDevice_T* selectedPhysicalDevice
);

struct VkPhysicalDevice_T* select_physical_device(
    struct VkPhysicalDevice_T** physicalDevices,
    uint32_t deviceCount
);

uint32_t get_queue_family_property_count(
    struct VkPhysicalDevice_T* selectedPhysicalDevice
);

struct VkQueueFamilyProperties* get_queue_family_properties(
    struct VkPhysicalDevice_T* selectedPhysicalDevice,
    uint32_t queueFamilyPropertyCount
);

void free_queue_family_properties(
    struct VkQueueFamilyProperties* queueFamilyProperties
);

uint32_t get_compute_queue_family_index(
    struct VkQueueFamilyProperties* queueFamilyProperties,
    uint32_t queueFamilyPropertyCount
);

struct VkDevice_T* get_logical_device(
    struct VkPhysicalDevice_T* selectedPhysicalDevice,
    struct VkDeviceCreateInfo* deviceInfo
);

struct VkQueue_T* get_logical_device_queue(
    struct VkDevice_T* vkDevice, uint32_t queueFamilyIndex
);

vulkan_device_t* create_vulkan_device(vulkan_instance_t* vkInstance);

void destroy_vulkan_device(vulkan_device_t* vkDevice);

#endif // VK_DEVICE
