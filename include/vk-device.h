/**
 * Copyright © 2024 Austin Berrio
 *
 * @file include/vk-device.h
 *
 * @brief Vulkan Device API for managing Vulkan devices and queues.
 *
 * @note This API handles physical and logical device management, queue setup,
 *       and other device-related operations.
 */

#ifndef VK_DEVICE
#define VK_DEVICE

#include "vk-instance.h"

/**
 * @brief Custom Vulkan Device and Queue structures to centralize related information. 
 */

// Alias for struct VkDevice_T* to distinguish logical devices
typedef struct VkDevice_T* VkLogicalDevice;

// @brief Structure representing a Vulkan device and related objects.
typedef struct VulkanDevice {
    uint32_t physicalCount; ///< Number of physical devices available
    VkPhysicalDevice* list;
    VkPhysicalDevice physical; ///< Selected physical device handle
    VkPhysicalDeviceProperties* properties; ///< Properties of the selected physical device
    VkDeviceCreateInfo* deviceInfo;
    VkLogicalDevice logical; ///< Logical device handle
} vulkan_device_t;

// @brief Structure representing a Vulkan queue and its properties.
typedef struct VulkanQueue {
    VkQueue handle; ///< Handle to the Vulkan queue
    uint32_t queueFamilyIndex; ///< Index of the queue family
    uint32_t queueFamilyPropertyCount; ///< Number of properties in the queue family
    VkQueueFamilyProperties* queueFamilyProperties; ///< Properties of the queue family
    VkDeviceQueueCreateInfo* deviceQueueInfo; ///< Queue creation info
} vulkan_queue_t;

/**
 * @brief Vulkan Device API that handles physical and logical device management.
 */

// @brief Creates and initializes VkDeviceCreateInfo structure.
struct VkDeviceCreateInfo* create_device_info(void);

// @brief Enumerates the number of physical devices available in the instance.
uint32_t enumerate_physical_device_count(struct VkInstance_T* vkInstance);

// @brief Enumerates the physical devices available in the instance.
struct VkPhysicalDevice_T** enumerate_physical_devices(
    struct VkInstance_T* vkInstance, uint32_t deviceCount
);

// @brief Frees the allocated physical device handles array.
void free_physical_device_handles(struct VkPhysicalDevice_T** physicalDeviceList);

// @brief Retrieves properties of the selected physical device.
struct VkPhysicalDeviceProperties* get_physical_device_properties(
    struct VkPhysicalDevice_T* selectedPhysicalDevice
);

// @brief Selects an appropriate physical device from the available devices.
struct VkPhysicalDevice_T* select_physical_device(
    struct VkPhysicalDevice_T** physicalDeviceList,
    uint32_t deviceCount
);

// @brief Creates a logical device from the selected physical device.
struct VkDevice_T* get_logical_device(
    struct VkPhysicalDevice_T* selectedPhysicalDevice,
    struct VkDeviceCreateInfo* deviceInfo
);

// @brief Initializes the VulkanDevice structure.
vulkan_device_t* create_vulkan_device(vulkan_instance_t* vkInstance);

// @brief Cleans up and destroys the VulkanDevice structure.
void destroy_vulkan_device(vulkan_device_t* vkDevice);

/**
 * @brief Vulkan Queue API that handles setup
 */

// @brief Creates and initializes VkDeviceQueueCreateInfo structure.
struct VkDeviceQueueCreateInfo* create_device_queue_info(void);

// @brief Retrieves the count of queue family properties for the physical device.
uint32_t get_queue_family_property_count(
    struct VkPhysicalDevice_T* selectedPhysicalDevice
);

// @brief Retrieves the queue family properties for the physical device.
struct VkQueueFamilyProperties* get_queue_family_properties(
    struct VkPhysicalDevice_T* selectedPhysicalDevice,
    uint32_t queueFamilyPropertyCount
);

// @brief Frees the allocated queue family properties array.
void free_queue_family_properties(
    struct VkQueueFamilyProperties* queueFamilyProperties
);

// @brief Finds the index of a queue family that supports compute operations.
uint32_t get_compute_queue_family_index(
    struct VkQueueFamilyProperties* queueFamilyProperties,
    uint32_t queueFamilyPropertyCount
);

// @brief Retrieves a handle to a specific queue from the logical device.
struct VkQueue_T* get_logical_device_queue(
    struct VkDevice_T* vkDevice, uint32_t queueFamilyIndex
);

// @brief Initializes the VulkanQueue structure.
vulkan_queue_t* create_vulkan_queue(vulkan_device_t* vkDevice);

// @brief Cleans up and destroys the VulkanQueue structure.
void destroy_vulkan_queue(vulkan_queue_t* vkQueue);

#endif // VK_DEVICE
