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

uint32_t get_physical_device_count(struct VkInstance_T* pVkInstance) {
    uint32_t deviceCount = 0;
    enum VkResult result = vkEnumeratePhysicalDevices(pVkInstance, &deviceCount, NULL);

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

struct VkPhysicalDevice_T** create_physical_devices(struct VkInstance_T* pVkInstance, uint32_t deviceCount) {
    struct VkPhysicalDevice_T** pPhysicalDevices = (struct VkPhysicalDevice_T**)
        malloc(deviceCount * sizeof(struct VkPhysicalDevice_T*));

    if (VK_SUCCESS != vkEnumeratePhysicalDevices(pVkInstance, &deviceCount, pPhysicalDevices)) {
        fprintf(stderr, "Failed to enumerate physical devices!\n");
        free(pPhysicalDevices);  // Clean up on failure
        return NULL;
    }

    return pPhysicalDevices;  // Return the list of devices
}

void destroy_physical_devices(struct VkPhysicalDevice_T** pPhysicalDevices) {
    if (pPhysicalDevices) {
        free(pPhysicalDevices);
    }
}
