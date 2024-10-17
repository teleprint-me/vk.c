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

struct VkDeviceCreateInfo create_device_info(void) {
    struct VkDeviceCreateInfo deviceInfo = {};
    deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    // Can be updated with queue info later
    deviceInfo.queueCreateInfoCount = 0;
    deviceInfo.pQueueCreateInfos = NULL;
    // No device extensions by default
    deviceInfo.enabledExtensionCount = 0;
    deviceInfo.ppEnabledExtensionNames = NULL;
    // No validation layers by default
    deviceInfo.enabledLayerCount = 0;
    deviceInfo.ppEnabledLayerNames = NULL;
    // Leave this as NULL until specific features are needed
    deviceInfo.pEnabledFeatures = NULL;
    return deviceInfo;
}

struct VkDeviceQueueCreateInfo create_device_queue_info(void) {
    // Default priority
    static float queuePriority = 1.0f;
    struct VkDeviceQueueCreateInfo deviceQueueInfo = {};
    deviceQueueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    // Default to the first queue family
    deviceQueueInfo.queueFamilyIndex = 0;
    // Start with one queue for now
    deviceQueueInfo.queueCount = 1;
    // Required field
    deviceQueueInfo.pQueuePriorities = &queuePriority;
    return deviceQueueInfo;
}

uint32_t get_physical_device_count(struct VkInstance_T* vkInstance) {
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

struct VkPhysicalDevice_T** create_physical_devices(
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

void destroy_physical_devices(struct VkPhysicalDevice_T** physicalDevices) {
    if (physicalDevices) {
        free(physicalDevices);
    }
}

struct VkPhysicalDeviceProperties get_physical_device_properties(
    struct VkPhysicalDevice_T* selectedPhysicalDevice, uint32_t deviceCount
) {
    struct VkPhysicalDeviceProperties physicalDeviceProperties;
    vkGetPhysicalDeviceProperties(selectedPhysicalDevice, &physicalDeviceProperties);
    return physicalDeviceProperties;
}

// attempt to guess which device should be returned
struct VkPhysicalDevice_T* select_physical_device(
    struct VkPhysicalDevice_T** physicalDevices, uint32_t deviceCount
) {
    for (uint32_t i = 0; i < deviceCount; i++) {
        struct VkPhysicalDeviceProperties deviceProperties = get_physical_device_properties(
            physicalDevices[i], deviceCount
        );
        // If device supports compute (discrete GPU preferred)
        if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
            return physicalDevices[i];
        }
    }

    // Fallback to first available device
    return physicalDevices[0];
}

uint32_t get_physical_device_queue_family_count(
    struct VkPhysicalDevice_T* selectedPhysicalDevice
) {
    uint32_t queueFamilyPropertyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(
        selectedPhysicalDevice, &queueFamilyPropertyCount, NULL
    );
    return queueFamilyPropertyCount;
}

struct VkQueueFamilyProperties* create_queue_family_properties(
    struct VkPhysicalDevice_T* selectedPhysicalDevice, uint32_t queueFamilyPropertyCount
) {
    struct VkQueueFamilyProperties* queueFamilyProperties = malloc(
        queueFamilyPropertyCount * sizeof(struct VkQueueFamilyProperties)
    );
    vkGetPhysicalDeviceQueueFamilyProperties(
        selectedPhysicalDevice, &queueFamilyPropertyCount, queueFamilyProperties
    );
    return queueFamilyProperties;
}

void destroy_queue_family_properties(
    struct VkQueueFamilyProperties* queueFamilyProperties
) {
    if (queueFamilyProperties) {
        free(queueFamilyProperties);
    }
}

uint32_t get_physical_device_queue_family(
    struct VkQueueFamilyProperties* queueFamilyProperties, uint32_t queueFamilyPropertyCount
) {
    for (uint32_t i = 0; i < queueFamilyPropertyCount; i++) {
        if (queueFamilyProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT) {
            return i; // Found a queue that supports compute operations
        }
    }

    // Fallback to first available queue family
    return 0;
}

struct VkDevice_T* create_logical_device(
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

struct VkQueue_T* get_logical_device_queue(
    struct VkDevice_T* logicalDevice, uint32_t queueFamilyIndex
) {
    // Retrieve the queue for compute operations
    struct VkQueue_T* computeQueue;
    vkGetDeviceQueue(logicalDevice, queueFamilyIndex, 0, &computeQueue);
    return computeQueue;
}
