/**
 * Copyright © 2024 Austin Berrio
 *
 * @file examples/simple.c
 * 
 * @note Apply zero-initialization strategy to maintain a "sane" default implementation.
 */

#include "vk-instance.h"
#include "vk-device.h"

#include <vulkan/vulkan.h>

#include <stdio.h>
#include <stdlib.h>

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

    // Get queue family properties for the selected device
    uint32_t queueFamilyCount = get_physical_device_queue_family_count(selectedPhysicalDevice);
    struct VkQueueFamilyProperties* queueFamilyProperties = create_queue_family_properties(selectedPhysicalDevice, queueFamilyCount);

    // Select the compute queue family
    uint32_t computeQueueFamilyIndex = get_physical_device_queue_family(queueFamilyProperties, queueFamilyCount);

    // Create a logical device
    struct VkDevice_T* logicalDevice;  // Logical device handle
    enum VkResult result = vkCreateDevice(
        selectedPhysicalDevice, &deviceInfo, NULL, &logicalDevice
    );

    if (VK_SUCCESS != result) {
        fprintf(stderr, "Failed to create logical device! (Error code: %d)\n", result);
        exit(EXIT_FAILURE);
    }

    // Retrieve the queue for compute operations
    struct VkQueue_T* computeQueue;
    vkGetDeviceQueue(logicalDevice, computeQueueFamilyIndex, 0, &computeQueue);

    // Clean up logical device
    vkDestroyDevice(logicalDevice, NULL);

    // Clean up queue properties
    destroy_queue_family_properties(queueFamilyProperties);

    // Clean up physical devices
    destroy_physical_devices(physicalDevices);

    // Clean up Vulkan instance
    destroy_vulkan_instance(vkInstance);

    return 0;
}
