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
    struct VkDeviceCreateInfo* deviceInfo = (struct VkDeviceCreateInfo*) malloc(
        sizeof(struct VkDeviceCreateInfo)
    );
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
    struct VkPhysicalDevice_T** physicalDeviceList = (struct VkPhysicalDevice_T**) malloc(
            deviceCount * sizeof(struct VkPhysicalDevice_T*)
    );

    if (VK_SUCCESS != vkEnumeratePhysicalDevices(vkInstance, &deviceCount, physicalDeviceList)) {
        fprintf(stderr, "Failed to enumerate physical devices!\n");
        free(physicalDeviceList);  // Clean up on failure
        return NULL;
    }

    return physicalDeviceList;  // Return the list of devices
}

void free_physical_device_handles(struct VkPhysicalDevice_T** physicalDeviceList) {
    if (physicalDeviceList) {
        free(physicalDeviceList);
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
    struct VkPhysicalDevice_T** physicalDeviceList, uint32_t deviceCount
) {
    for (uint32_t i = 0; i < deviceCount; i++) {
        struct VkPhysicalDeviceProperties* deviceProperties = get_physical_device_properties(
            physicalDeviceList[i]
        );

        // If device supports compute (discrete GPU preferred)
        if (VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU == deviceProperties->deviceType) {
            return physicalDeviceList[i];
        }
    }

    // Fallback to first available device
    return physicalDeviceList[0];
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
    struct VkDeviceQueueCreateInfo* deviceQueueInfo = (struct VkDeviceQueueCreateInfo*) malloc(
        sizeof(struct VkDeviceQueueCreateInfo)
    );
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

/**
 * @brief Create and destroy Vulkan Queue
 */

vulkan_queue_t* create_vulkan_queue(vulkan_device_t* vkDevice) {
    vulkan_queue_t* vkQueue = (vulkan_queue_t*) malloc(sizeof(vulkan_queue_t));

    // Create and set queue creation info
    vkQueue->deviceQueueInfo = create_device_queue_info();

    // Get queue family property count and properties
    vkQueue->queueFamilyPropertyCount = get_queue_family_property_count(vkDevice->physical);
    vkQueue->queueFamilyProperties = get_queue_family_properties(
        vkDevice->physical, vkQueue->queueFamilyPropertyCount
    );

    // Find the compute-capable queue family index
    vkQueue->queueFamilyIndex = get_compute_queue_family_index(
        vkQueue->queueFamilyProperties, vkQueue->queueFamilyPropertyCount
    );

    // Set the queue family index in the device queue info
    vkQueue->deviceQueueInfo->queueFamilyIndex = vkQueue->queueFamilyIndex;

    // Retrieve the Vulkan queue handle from the logical device
    vkGetDeviceQueue(vkDevice->logical, vkQueue->queueFamilyIndex, 0, &vkQueue->handle);

    return vkQueue;
}

void destroy_vulkan_queue(vulkan_queue_t* vkQueue) {
    if (vkQueue) {
        // Free queue family properties if allocated
        if (vkQueue->queueFamilyProperties) {
            free_queue_family_properties(vkQueue->queueFamilyProperties);
        }
        // Free the device queue info
        if (vkQueue->deviceQueueInfo) {
            free(vkQueue->deviceQueueInfo);
        }
        // Finally, free the queue structure itself
        free(vkQueue);
    }
}

/**
 * @brief Create and destroy Vulkan Device
 */

vulkan_device_t* create_vulkan_device(vulkan_instance_t* vkInstance) {
    vulkan_device_t* vkDevice = (vulkan_device_t*) malloc(sizeof(vulkan_device_t));
    
    // Get the physical device count and select the best physical device
    vkDevice->physicalCount = enumerate_physical_device_count(vkInstance->handle);
    vkDevice->list = enumerate_physical_devices(vkInstance->handle, vkDevice->physicalCount);
    vkDevice->physical = select_physical_device(vkDevice->list, vkDevice->physicalCount);
    vkDevice->properties = get_physical_device_properties(vkDevice->physical);
    vkDevice->deviceInfo = create_device_info();
    vkDevice->logical = get_logical_device(vkDevice->physical, vkDevice->deviceInfo);

    return vkDevice;
}

void destroy_vulkan_device(vulkan_device_t* vkDevice) {
    // Memory cleanup
    if (vkDevice) {
        if (vkDevice->list) {
            free_physical_device_handles(vkDevice->list);
        }
        vkDestroyDevice(vkDevice->logical, NULL);
    }
}
