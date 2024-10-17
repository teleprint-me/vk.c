/**
 * @file examples/simple.c
 * 
 * @note Apply zero-initialization strategy to maintain a "sane" default implementation.
 */

#include "vk-instance.h"

#include <vulkan/vulkan.h>

#include <stdio.h>
#include <stdlib.h>

// ***
// Create the Vulkan device object
// ***

VkDeviceCreateInfo create_device_info(void) {
    struct VkDeviceCreateInfo device_info = {};
    device_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    device_info.queueCreateInfoCount = 0;  // Can be updated with queue info later
    device_info.pQueueCreateInfos = NULL;
    device_info.enabledExtensionCount = 0;  // No device extensions by default
    device_info.ppEnabledExtensionNames = NULL;
    device_info.enabledLayerCount = 0;  // No validation layers by default
    device_info.ppEnabledLayerNames = NULL;
    device_info.pEnabledFeatures = NULL;  // Leave this as NULL until specific features are needed
    return device_info;
}

struct VkDeviceQueueCreateInfo create_device_queue_info(void) {
    static float queuePriority = 1.0f; // Default priority
    struct VkDeviceQueueCreateInfo queueCreateInfo = {};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = 0; // Default to the first queue family
    queueCreateInfo.queueCount = 1; // Start with one queue for now
    queueCreateInfo.pQueuePriorities = &queuePriority; // Required field
    return queueCreateInfo;
}

uint32_t get_physical_device_count(struct VkInstance_T* pVkInstance) {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(pVkInstance, &deviceCount, NULL);

    if (0 == deviceCount) {
        fprintf(stderr, "Failed to find GPUs with Vulkan support!\n");
        exit(EXIT_FAILURE);
    }

    return deviceCount;
}

struct VkPhysicalDevice_T* create_physical_devices(struct VkInstance_T* pVkInstance, uint32_t deviceCount) {
    struct VkPhysicalDevice_T* pPhysicalDevices = malloc(deviceCount * sizeof(struct VkPhysicalDevice_T*));
    vkEnumeratePhysicalDevices(pVkInstance, &deviceCount, &pPhysicalDevices);
    return pPhysicalDevices;
}

void destroy_physical_devices(struct VkPhysicalDevice_T* pPhysicalDevices) {
    if (pPhysicalDevices) {
        free(pPhysicalDevices);
    }
}

struct VkPhysicalDeviceProperties get_physical_device_properties(struct VkPhysicalDevice_T* pPhysicalDevice, uint32_t deviceCount) {
    struct VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(pPhysicalDevice, &deviceProperties);
    return deviceProperties;
}

// attempt to guess which device should be returned
struct VkPhysicalDevice_T * get_physical_device(
    struct VkInstance_T* pVkInstance, struct VkPhysicalDevice_T* pPhysicalDevices, uint32_t deviceCount
) {
    for (uint32_t i = 0; i < deviceCount; i++) {
        VkPhysicalDeviceProperties deviceProperties
            = get_physical_device_properties((&pPhysicalDevices)[i], deviceCount);
        // If device supports compute (discrete GPU preferred)
        if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
            return (&pPhysicalDevices)[i];
        }
    }

    // Fallback to first available device
    return (&pPhysicalDevices)[0];
}

uint32_t get_physical_device_queue_family_count(struct VkPhysicalDevice_T* pVkPhysicalDevice) {
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(pVkPhysicalDevice, &queueFamilyCount, NULL);
    return queueFamilyCount;
}

struct VkQueueFamilyProperties* create_queue_family_properties(
    struct VkPhysicalDevice_T* pPhysicalDevice, uint32_t queueFamilyCount
) {
    struct VkQueueFamilyProperties* queueFamilyProperties = malloc(queueFamilyCount * sizeof(struct VkQueueFamilyProperties));
    vkGetPhysicalDeviceQueueFamilyProperties(pPhysicalDevice, &queueFamilyCount, queueFamilyProperties);
    return queueFamilyProperties;
}

void destroy_queue_family_properties(struct VkQueueFamilyProperties* pPhysicalDevices) {
    if (pPhysicalDevices) {
        free(pPhysicalDevices);
    }
}

uint32_t get_physical_device_queue_family(struct VkPhysicalDevice_T* pVkPhysicalDevice, uint32_t queueFamilyCount) {
    VkQueueFamilyProperties queueFamilies[queueFamilyCount];
    vkGetPhysicalDeviceQueueFamilyProperties(pVkPhysicalDevice, &queueFamilyCount, queueFamilies);

    for (uint32_t i = 0; i < queueFamilyCount; i++) {
        if (queueFamilies[i].queueFlags & VK_QUEUE_COMPUTE_BIT) {
            return i; // Found a queue that supports compute operations
        }
    }

    // Fallback to first available queue family
    return 0;
}

// @note we only need a help text and the ability to pass a file path to the
// shader being utilized for compute operations.
int main(int argc, char* argv[]) {
    // Create the Vulkan instance objects
    vulkan_instance_t* vkInstance = create_vulkan_instance("SimpleApp", "SimpleEngine");

    // Create the Vulkan device instance objects
    uint32_t deviceCount = get_physical_device_count(vkInstance->handle);
    struct VkPhysicalDevice_T* pPhysicalDevices = create_physical_devices(vkInstance->handle, deviceCount);

    // Pick the best physical device (discrete GPU preferred)
    struct VkPhysicalDevice_T* physicalDevice = get_physical_device(vkInstance->handle, pPhysicalDevices, deviceCount);
    uint32_t queueFamilyCount = get_physical_device_queue_family_count(physicalDevice);
    uint32_t queueFamilyIndex = get_physical_device_queue_family(physicalDevice, queueFamilyCount);

    // Create a device (placeholder)
    VkDevice vkDevice = create_vk_device(physicalDevice, queueFamilyIndex);

    // Clean up
    destroy_physical_devices(pPhysicalDevices);
    destroy_vulkan_instance(vkInstance);
    // vkDestroyDevice(vkDevice, NULL); // Once device creation is implemented

    return 0;
}
