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

VkDeviceCreateInfo vulkan_create_device_info(void) {
    VkDeviceCreateInfo deviceInfo = {0};
    deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceInfo.queueCreateInfoCount = 0;
    deviceInfo.pQueueCreateInfos = NULL;
    deviceInfo.enabledExtensionCount = 0;
    deviceInfo.ppEnabledExtensionNames = NULL;
    deviceInfo.enabledLayerCount = 0;
    deviceInfo.ppEnabledLayerNames = NULL;
    deviceInfo.pEnabledFeatures = NULL;
    return deviceInfo;
}

uint32_t vulkan_enumerate_physical_device_count(VkInstance vkInstance) {
    uint32_t deviceCount = 0;
    VkResult result = vkEnumeratePhysicalDevices(vkInstance, &deviceCount, NULL);

    if (VK_SUCCESS != result) {
        fprintf(stderr, "Failed to enumerate physical devices! (Error code: %d)\n");
        exit(EXIT_FAILURE);
    }

    if (deviceCount == 0) {
        fprintf(stderr, "No GPUs with Vulkan support found!\n");
        exit(EXIT_FAILURE);
    }

    return deviceCount;
}

VkPhysicalDevice* vulkan_enumerate_physical_device_list(VkInstance vkInstance, uint32_t deviceCount) {
    VkPhysicalDevice* physicalDeviceList = (VkPhysicalDevice*) malloc(
        deviceCount * sizeof(VkPhysicalDevice)
    );

    if (physicalDeviceList == NULL) {
        fprintf(stderr, "Failed to allocate memory for physical device list!\n");
        return NULL;
    }

    VkResult result = vkEnumeratePhysicalDevices(vkInstance, &deviceCount, physicalDeviceList);
    if (VK_SUCCESS != result) {
        fprintf(stderr, "Failed to enumerate physical devices! (Error code: %d)\n", result);
        free(physicalDeviceList);
        return NULL;
    }

    return physicalDeviceList;
}

void vulkan_free_device_list(VkPhysicalDevice* physicalDeviceList) {
    if (physicalDeviceList != NULL) {
        free(physicalDeviceList);
    }
}

VkPhysicalDeviceProperties vulkan_get_physical_device_properties(
    VkPhysicalDevice selectedPhysicalDevice
) {
    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(selectedPhysicalDevice, &properties);
    return properties;
}

// attempt to guess which device should be returned
VkPhysicalDevice vulkan_select_physical_device(
    VkPhysicalDevice* physicalDeviceList, uint32_t deviceCount
) {
    for (uint32_t i = 0; i < deviceCount; i++) {
        VkPhysicalDeviceProperties properties = vulkan_get_physical_device_properties(physicalDeviceList[i]);

        if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
            return physicalDeviceList[i];
        }
    }

    // Fallback to first available device
    return physicalDeviceList[0];
}

VkDevice vulkan_create_logical_device(
    VkPhysicalDevice selectedPhysicalDevice, VkDeviceCreateInfo deviceInfo
) {
    VkDevice logicalDevice;
    VkResult result = vkCreateDevice(selectedPhysicalDevice, &deviceInfo, NULL, &logicalDevice);

    if (VK_SUCCESS != result) {
        fprintf(stderr, "Failed to create logical device! (Error code: %d)\n", result);
        exit(EXIT_FAILURE);
    }

    return logicalDevice;
}

vulkan_device_t* vulkan_create_device(VkInstance vkInstance) {
    vulkan_device_t* vkDevice = (vulkan_device_t*) malloc(sizeof(vulkan_device_t));

    // Enumerate and select the physical device
    vkDevice->physicalDeviceCount = vulkan_enumerate_physical_device_count(vkInstance);
    VkPhysicalDevice* physicalDeviceList = vulkan_enumerate_physical_device_list(
        vkInstance, vkDevice->physicalDeviceCount
    );
    vkDevice->physicalDevice = vulkan_select_physical_device(
        physicalDeviceList, vkDevice->physicalDeviceCount
    );
    vulkan_free_device_list(physicalDeviceList);

    // Get device properties
    vkDevice->physicalDeviceProperties = vulkan_get_physical_device_properties(
        vkDevice->physicalDevice
    );

    // Create the logical device
    vkDevice->deviceInfo = vulkan_create_device_info();
    vkDevice->logicalDevice = vulkan_create_logical_device(
        vkDevice->physicalDevice, vkDevice->deviceInfo
    );

    return vkDevice;
}

void vulkan_destroy_device(vulkan_device_t* vkDevice) {
    if (vkDevice != NULL) {
        if (vkDevice->logicalDevice != VK_NULL_HANDLE) {
            vkDestroyDevice(vkDevice->logicalDevice, NULL);
        }
        free(vkDevice);
    }
}

/**
 * @brief Vulkan Queue API that handles setup
 */

VkDeviceQueueCreateInfo vulkan_create_device_queue_info(uint32_t queueFamilyIndex) {
    static float queuePriority = 1.0f;
    VkDeviceQueueCreateInfo queueInfo = {0};
    queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueInfo.queueFamilyIndex = queueFamilyIndex;
    queueInfo.queueCount = 1;  // Using a single queue
    queueInfo.pQueuePriorities = &queuePriority;  // Single priority value for one queue
    return queueInfo;
}

uint32_t vulkan_get_queue_family_property_count(VkPhysicalDevice selectedPhysicalDevice) {
    uint32_t queueFamilyPropertyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(selectedPhysicalDevice, &queueFamilyPropertyCount, NULL);
    return queueFamilyPropertyCount;
}

void vulkan_get_queue_family_properties(
    VkPhysicalDevice selectedPhysicalDevice, VkQueueFamilyProperties* queueFamilyProperties
) {
    uint32_t queueFamilyPropertyCount = vulkan_get_queue_family_property_count(selectedPhysicalDevice);
    vkGetPhysicalDeviceQueueFamilyProperties(
        selectedPhysicalDevice, &queueFamilyPropertyCount, queueFamilyProperties
    );
}

uint32_t vulkan_get_compute_queue_family_index(
    VkQueueFamilyProperties* queueFamilyProperties, uint32_t queueFamilyPropertyCount
) {
    for (uint32_t i = 0; i < queueFamilyPropertyCount; i++) {
        if (queueFamilyProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT) {
            return i;  // Return the index of the compute-capable queue
        }
    }
    return 0;  // Fallback to first available queue family
}

vulkan_queue_t* vulkan_create_queue(vulkan_device_t* vkDevice) {
    vulkan_queue_t* vkQueue = (vulkan_queue_t*) malloc(sizeof(vulkan_queue_t));

    // Get the queue family properties on the stack
    uint32_t queueFamilyPropertyCount = vulkan_get_queue_family_property_count(vkDevice->physicalDevice);
    VkQueueFamilyProperties queueFamilyProperties[16];  // Assuming 16 families is sufficient
    vulkan_get_queue_family_properties(vkDevice->physicalDevice, queueFamilyProperties);

    // Find the compute-capable queue family index
    vkQueue->queueFamilyIndex = vulkan_get_compute_queue_family_index(queueFamilyProperties, queueFamilyPropertyCount);

    // Set up the queue creation info
    VkDeviceQueueCreateInfo queueCreateInfo = vulkan_create_device_queue_info(vkQueue->queueFamilyIndex);
    
    // Create the queue using vkGetDeviceQueue (no need for vkCreateQueue)
    vkGetDeviceQueue(vkDevice->logicalDevice, vkQueue->queueFamilyIndex, 0, &vkQueue->handle);

    return vkQueue;
}

void vulkan_destroy_queue(vulkan_queue_t* vkQueue) {
    if (vkQueue != NULL) {
        free(vkQueue);  // Only free the structure itself
    }
}

/**
 * @brief
 */

void initialize_device_queue(vulkan_device_t* vkDevice, vulkan_queue_t* vkQueue) {
    // We're setting up 1 queue for now
    vkDevice->deviceInfo->queueCreateInfoCount = 1;

    // Set the queue family index in the device queue info
    vkQueue->deviceQueueInfo->queueFamilyIndex = vkQueue->queueFamilyIndex;

    // Pass the queue create info
    vkDevice->deviceInfo->pQueueCreateInfos = vkQueue->deviceQueueInfo;

    // Create a logical device
    enum VkResult result = vkCreateDevice(
        vkDevice->physical, vkDevice->deviceInfo, NULL, vkDevice->logical
    );

    if (VK_SUCCESS != result) {
        fprintf(stderr, "Failed to create logical device! (Error code: %d)\n", result);
        exit(EXIT_FAILURE);
    }

    // Retrieve the Vulkan queue handle from the logical device
    vkGetDeviceQueue(vkDevice->logical, vkQueue->queueFamilyIndex, 0, &vkQueue->handle);
}
