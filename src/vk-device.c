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
    deviceInfo.queueCreateInfoCount = 0;  // Can be updated with queue info later
    deviceInfo.pQueueCreateInfos = NULL;
    deviceInfo.enabledExtensionCount = 0;  // No device extensions by default
    deviceInfo.ppEnabledExtensionNames = NULL;
    deviceInfo.enabledLayerCount = 0;  // No validation layers by default
    deviceInfo.ppEnabledLayerNames = NULL;
    deviceInfo.pEnabledFeatures = NULL;  // Leave this as NULL until specific features are needed
    return deviceInfo;
}

struct VkDeviceQueueCreateInfo create_device_queue_info(void) {
    static float queuePriority = 1.0f; // Default priority
    struct VkDeviceQueueCreateInfo deviceQueueInfo = {};
    deviceQueueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    deviceQueueInfo.queueFamilyIndex = 0; // Default to the first queue family
    deviceQueueInfo.queueCount = 1; // Start with one queue for now
    deviceQueueInfo.pQueuePriorities = &queuePriority; // Required field
    return deviceQueueInfo;
}
