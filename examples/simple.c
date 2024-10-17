/**
 * @file examples/simple.c
 * 
 * @note Apply zero-initialization strategy to maintain a "sane" default implementation.
 */

#include "vk-instance.h"
#include "vk-device.h"

#include <vulkan/vulkan.h>

#include <stdio.h>
#include <stdlib.h>

// ***
// Create the Vulkan device object
// ***

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
int main(void) {
    // Create Vulkan instance
    vulkan_instance_t* vkInstance = create_vulkan_instance("SimpleApp", "SimpleEngine");

    // Create device-related info objects
    struct VkDeviceCreateInfo deviceInfo = create_device_info();
    struct VkDeviceQueueCreateInfo deviceQueueInfo = create_device_queue_info();

    // Get the physical device count
    uint32_t deviceCount = get_physical_device_count(vkInstance->handle);

    // Enumerate and create physical devices
    struct VkPhysicalDevice_T** physicalDevices = create_physical_devices(vkInstance->handle, deviceCount);

    // Select the best physical device
    struct VkPhysicalDevice_T* selectedPhysicalDevice = select_physical_device(physicalDevices, deviceCount);

    // Get properties for the selected physical device
    struct VkPhysicalDeviceProperties physicalDeviceProperties = get_physical_device_properties(selectedPhysicalDevice, deviceCount);

    // Pick the best physical device (discrete GPU preferred)
    uint32_t queueFamilyCount = get_physical_device_queue_family_count(pPhysicalDevice);
    uint32_t queueFamilyIndex = get_physical_device_queue_family(physicalDevice, queueFamilyCount);

    // Create a device (placeholder)
    VkDevice vkDevice = create_vk_device(physicalDevice, queueFamilyIndex);

    // Clean up
    destroy_physical_devices(pPhysicalDevices);
    destroy_vulkan_instance(vkInstance);
    // vkDestroyDevice(vkDevice, NULL); // Once device creation is implemented

    return 0;
}
