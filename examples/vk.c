/**
 * Copyright © 2024 Austin Berrio
 *
 * @file examples/simple.c
 *
 * @note The primary objective with this example program is to perform summation
 * using a discrete GPU with the Vulkan C API. The vk.c library is exposed to
 * showcase the underlying logic.
 *
 * @todo Need to enable debugging as the project scope grows before it becomes
 * an issue. It is currently omitted to intentionally keep things as simple
 * as possible due to the nature of how complex the Vulkan C API is.
 *
 * How to Build Manually:
 * gcc -o vk src/vk-instance.c src/vk-device.c examples/vk.c -I./include -lvulkan
 *
 * I need to rename the file because simple was to vague, and vk.c conflicts with
 * the repo name. So, manually building is temporarily required until I figure out
 * a better name.
 */

#include "vk-instance.h"
#include "vk-device.h"

#include <vulkan/vulkan.h>

#include <stdio.h>
#include <stdlib.h>

// @note we only need a help text and the ability to pass a file path to the
// shader being utilized for compute operations.
int main(void) {
    /**********************
     * Allocate resources *
     **********************/

    /**
     * Setup the Vulkan Instance Object
     */
    const char* applicationName = "ComputeApp";
    const char* engineName = "ComputeEngine";

    // Allocate memory to the custom vkInstance
    // @note We should not reference this object as the vkInstance. This is confusing.
    vulkan_instance_t* vkInstance = calloc(1, sizeof(vulkan_instance_t));
    if (NULL == vkInstance) {
        fprintf(stderr, "vulkan_create_instance: Failed to allocate memory for VulkanInstance\n");
        return EXIT_FAILURE;
    }

    // Initialize applicationInfo and instanceCreateInfo
    vkInstance->applicationInfo = vulkan_create_application_info(
        applicationName,
        engineName
    );
    vkInstance->instanceCreateInfo = vulkan_create_instance_info(&vkInstance->applicationInfo);

    // Example: Setting extensions and validation layers
    // For headless compute, you might not need any extensions or layers initially
    // Uncomment and modify the following lines as needed

    /*
    const char* extensions[] = {
        // Add required extensions here, e.g., "VK_KHR_surface", "VK_EXT_debug_utils"
    };
    vulkan_set_instance_info_extensions(&vkInstance->instanceCreateInfo, extensions, sizeof(extensions)/sizeof(extensions[0]));

    const char* layers[] = {
        // Add validation layers here if needed, e.g., "VK_LAYER_KHRONOS_validation"
    };
    vulkan_set_instance_info_validation_layers(&vkInstance->instanceCreateInfo, layers, sizeof(layers)/sizeof(layers[0]));
    */

    // Create the Vulkan instance
    VkResult result = vkCreateInstance(
        &vkInstance->instanceCreateInfo,
        NULL, // Allocation callbacks (optional)
        &vkInstance->handle
    );

    if (VK_SUCCESS != result) {
        fprintf(
            stderr,
            "vulkan_create_instance: Failed to create Vulkan instance! (Error code: %d)\n",
            result
        );
        free(vkInstance);
        return EXIT_FAILURE;
    }

    /**
     * Setup the Vulkan Device Object
     * 
     * @todo Add example comments for extensions and validation layers
     */
    // Allocate memory to custom vkDevice
    // @note We should not reference this object as the vkDevice. This is confusing.
    vulkan_device_t* vkDevice = (vulkan_device_t*) malloc(sizeof(vulkan_device_t));

    // Enumerate and select the physical device
    vkDevice->physicalDeviceCount = vulkan_enumerate_physical_device_count(vkInstance->handle);
    VkPhysicalDevice* physicalDeviceList = vulkan_enumerate_physical_device_list(
        vkInstance->handle, vkDevice->physicalDeviceCount
    );
    vkDevice->physicalDevice = vulkan_select_physical_device(
        physicalDeviceList, vkDevice->physicalDeviceCount
    );
    vulkan_free_device_list(physicalDeviceList);

    // Get device properties
    vkDevice->physicalDeviceProperties = vulkan_get_physical_device_properties(
        vkDevice->physicalDevice
    );

    // Create the queue create info on the stack
    uint32_t queueFamilyPropertyCount = vulkan_get_queue_family_property_count(vkDevice->physicalDevice);
    VkQueueFamilyProperties queueFamilyProperties[16];
    vulkan_get_queue_family_properties(vkDevice->physicalDevice, queueFamilyProperties);
    
    uint32_t computeQueueFamilyIndex = vulkan_get_compute_queue_family_index(queueFamilyProperties, queueFamilyPropertyCount);

    // Create queue creation info
    VkDeviceQueueCreateInfo queueCreateInfo = vulkan_create_device_queue_info(computeQueueFamilyIndex);

    // Set up the device creation info
    VkDeviceCreateInfo deviceCreateInfo = vulkan_create_device_info(&queueCreateInfo, 1);

    // Create the logical device
    result = vkCreateDevice(vkDevice->physicalDevice, &deviceCreateInfo, NULL, &vkDevice->logicalDevice);
    if (VK_SUCCESS != result) {
        fprintf(stderr, "Failed to create logical device! (Error code: %d)\n", result);
        exit(EXIT_FAILURE);
    }

    // Retrieve the queue handle from the logical device
    vkGetDeviceQueue(vkDevice->logicalDevice, computeQueueFamilyIndex, 0, &vkDevice->logicalQueue);

    // Create the queue structure and assign the handle
    vkDevice->queueFamilyIndex = computeQueueFamilyIndex;

    /**
     * Output status
     */
    printf("Silence is golden <3\n");

    /********************************
     * Clean up allocated resources *
     ********************************/

    /** 
     * Destroy the Vulkan Device Object
     */
    if (vkDevice != NULL) {
        if (vkDevice->logicalDevice != VK_NULL_HANDLE) {
            vkDestroyDevice(vkDevice->logicalDevice, NULL);
        }
        free(vkDevice);
    }

    /** 
     * Destroy the Vulkan Instance Object
     */
    if (NULL == vkInstance) {
        fprintf(stderr, "vulkan_destroy_instance: vkInstance is NULL\n");
        return EXIT_FAILURE;
    }

    if (VK_NULL_HANDLE != vkInstance->handle) {
        vkDestroyInstance(vkInstance->handle, NULL); // Vulkan instance cleanup
    }

    free(vkInstance);

    return EXIT_SUCCESS;
}
