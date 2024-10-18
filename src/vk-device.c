/**
 * Copyright © 2024 Austin Berrio
 *
 * @file src/vk-device.c
 *
 * @brief Vulkan Device API for managing Vulkan instances and devices.
 *
 * @note This API handles physical and logical device management, queue setup,
 *       and other device-related operations.
 */

#include "vk-device.h"

#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Vulkan Device API that handles physical and logical device management.
 */

struct VkDeviceCreateInfo* create_device_info(void) {
    struct VkDeviceCreateInfo* deviceInfo = (struct VkDeviceCreateInfo*) malloc(sizeof(struct VkDeviceCreateInfo));
    deviceInfo->sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceInfo->queueCreateInfoCount = 0;
    deviceInfo->pQueueCreateInfos = NULL;
    deviceInfo->enabledExtensionCount = 0;
    deviceInfo->ppEnabledExtensionNames = NULL;
    deviceInfo->enabledLayerCount = 0;
    deviceInfo->ppEnabledLayerNames = NULL;
    deviceInfo->pEnabledFeatures = NULL;
    return deviceInfo;
}

uint32_t enumerate_physical_device_count(struct VkInstance_T* vkInstance) {
    uint32_t deviceCount = 0;
    enum VkResult result = vkEnumeratePhysicalDevices(vkInstance, &deviceCount, NULL);

    if (VK_SUCCESS != result) {
        fprintf(stderr, "Failed to enumerate physical devices! (Error code: %d)\n", result);
        exit(EXIT_FAILURE);
    }

    if (0 == deviceCount) {
        fprintf(stderr, "No GPUs with Vulkan support found!\n");
        exit(EXIT_FAILURE);
    }

    return deviceCount;
}

struct VkPhysicalDevice_T** enumerate_physical_devices(
    struct VkInstance_T* vkInstance, uint32_t deviceCount
) {
    struct VkPhysicalDevice_T** physicalDevices = (struct VkPhysicalDevice_T**)
        malloc(deviceCount * sizeof(struct VkPhysicalDevice_T*));

    if (VK_SUCCESS != vkEnumeratePhysicalDevices(vkInstance, &deviceCount, physicalDevices)) {
        fprintf(stderr, "Failed to enumerate physical devices!\n");
        free(physicalDevices);  // Clean up on failure
        return NULL;
    }

    return physicalDevices;  // Return the list of devices
}

void free_physical_device_handles(struct VkPhysicalDevice_T** physicalDevices) {
    if (physicalDevices) {
        free(physicalDevices);
    }
}

struct VkPhysicalDeviceProperties* get_physical_device_properties(
    struct VkPhysicalDevice_T* selectedPhysicalDevice
) {
    struct VkPhysicalDeviceProperties* physicalDeviceProperties;
    vkGetPhysicalDeviceProperties(selectedPhysicalDevice, physicalDeviceProperties);
    return physicalDeviceProperties;
}

// attempt to guess which device should be returned
struct VkPhysicalDevice_T* select_physical_device(
    struct VkPhysicalDevice_T** physicalDevices,
    uint32_t deviceCount
) {
    for (uint32_t i = 0; i < deviceCount; i++) {
        struct VkPhysicalDeviceProperties* deviceProperties = get_physical_device_properties(physicalDevices[i]);
        // If device supports compute (discrete GPU preferred)
        if (deviceProperties->deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
            return physicalDevices[i];
        }
    }

    // Fallback to first available device
    return physicalDevices[0];
}

struct VkDevice_T* get_logical_device(
    struct VkPhysicalDevice_T* selectedPhysicalDevice,
    struct VkDeviceCreateInfo* deviceInfo
) {
    // Create a logical device
    struct VkDevice_T* logicalDevice;  // Logical device handle
    enum VkResult result = vkCreateDevice(
        selectedPhysicalDevice, deviceInfo, NULL, &logicalDevice
    );

    if (VK_SUCCESS != result) {
        fprintf(stderr, "Failed to create logical device! (Error code: %d)\n", result);
        exit(EXIT_FAILURE);
    }

    return logicalDevice;
}

/**
 * @brief Vulkan Queue API that handles setup
 */

struct VkDeviceQueueCreateInfo* create_device_queue_info(void) {
    static float queuePriority = 1.0f;
    struct VkDeviceQueueCreateInfo* deviceQueueInfo = (struct VkDeviceQueueCreateInfo*) malloc(sizeof(struct VkDeviceQueueCreateInfo));
    deviceQueueInfo->sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    deviceQueueInfo->queueFamilyIndex = 0;
    deviceQueueInfo->queueCount = 1;
    deviceQueueInfo->pQueuePriorities = &queuePriority;
    return deviceQueueInfo;
}

uint32_t get_queue_family_property_count(
    struct VkPhysicalDevice_T* selectedPhysicalDevice
) {
    uint32_t queueFamilyPropertyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(
        selectedPhysicalDevice, &queueFamilyPropertyCount, NULL
    );
    return queueFamilyPropertyCount;
}

struct VkQueueFamilyProperties* get_queue_family_properties(
    struct VkPhysicalDevice_T* selectedPhysicalDevice,
    uint32_t queueFamilyPropertyCount
) {
    struct VkQueueFamilyProperties* queueFamilyProperties = malloc(
        queueFamilyPropertyCount * sizeof(struct VkQueueFamilyProperties)
    );
    vkGetPhysicalDeviceQueueFamilyProperties(
        selectedPhysicalDevice, &queueFamilyPropertyCount, queueFamilyProperties
    );
    return queueFamilyProperties;
}

void free_queue_family_properties(
    struct VkQueueFamilyProperties* queueFamilyProperties
) {
    if (queueFamilyProperties) {
        free(queueFamilyProperties);
    }
}

vulkan_queue_t* create_vulkan_queue(struct VkPhysicalDevice_T* selectedPhysicalDevice) {
    vulkan_queue_t* queue = (vulkan_queue_t*) malloc(sizeof(vulkan_queue_t));

    queue->deviceQueueInfo = create_device_queue_info();
    queue->queueFamilyPropertyCount = get_queue_family_property_count(selectedPhysicalDevice);

    return queue;
}

uint32_t get_compute_queue_family_index(
    struct VkQueueFamilyProperties* queueFamilyProperties,
    uint32_t queueFamilyPropertyCount
) {
    for (uint32_t i = 0; i < queueFamilyPropertyCount; i++) {
        if (queueFamilyProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT) {
            return i; // Found a queue that supports compute operations
        }
    }

    // Fallback to first available queue family
    return 0;
}

struct VkQueue_T* get_logical_device_queue(
    struct VkDevice_T* vkDevice, uint32_t queueFamilyIndex
) {
    // Retrieve the queue for compute operations
    struct VkQueue_T* queue;
    vkGetDeviceQueue(vkDevice, queueFamilyIndex, 0, &queue);
    return queue;
}

/**
 * Create and destroy Vulkan Queue
 */

vulkan_queue_t* create_vulkan_queue(struct VkPhysicalDevice_T* selectedPhysicalDevice) {
    vulkan_queue_t* queue = (vulkan_queue_t*) malloc(sizeof(vulkan_queue_t));
    queue->deviceQueueInfo = create_device_queue_info();
    queue->queueFamilyPropertyCount = get_queue_family_property_count(selectedPhysicalDevice);
    queue->queueFamilyProperties = get_queue_family_properties(selectedPhysicalDevice, queue->queueFamilyPropertyCount);

}

void destroy_vulkan_queue(vulkan_queue_t* vkQueue) {
    // @todo
}

/**
 * Create and destroy Vulkan Device
 */

vulkan_device_t* create_vulkan_device(vulkan_instance_t* vkInstance) {
    vulkan_device_t* device = (vulkan_device_t*) malloc(sizeof(vulkan_device_t));

    // Get the physical device count
    device->physicalCount = get_physical_device_count(vkInstance->handle);

    // Allocate and enumerate physical devices (you might already have a function for this)
    VkPhysicalDevice* physicalDevices = enumerate_physical_devices(
        vkInstance->handle, device->physicalCount
    );

    // Select the best physical device (if you have multiple, you can pick the most suitable)
    device->physical = select_physical_device(physicalDevices, device->physicalCount);

    // Get the queue family index for compute operations
    uint32_t queueFamilyIndex = get_compute_queue_family_index(
        get_queue_family_properties(device->physical, get_queue_family_property_count(device->physical)),
        get_queue_family_property_count(device->physical)
    );

    // Create and set up device queue info
    struct VkDeviceCreateInfo* deviceInfo = create_device_info();
    struct VkDeviceQueueCreateInfo* deviceQueueInfo = create_device_queue_info();

    // Set the queue family index in the queue creation info
    deviceQueueInfo->queueFamilyIndex = queueFamilyIndex;
    
    // Update device creation info with the queue information
    deviceInfo->queueCreateInfoCount = 1;
    deviceInfo->pQueueCreateInfos = deviceQueueInfo;

    // Final device setup steps, such as logical device creation...

    return device;
}

void destroy_vulkan_device(vulkan_device_t* device) {
    // @todo
}
