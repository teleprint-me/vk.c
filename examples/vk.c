/**
 * Copyright © 2024 Austin Berrio
 *
 * @file examples/simple.c
 * 
 * @note The primary objective with this example program is to perform summation
 * using a discrete GPU with the Vulkan C API. The vk.c library is exposed to
 * showcase the underlying logic.
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

    // Allocate memory with zero-initialization
    vulkan_instance_t* vkInstance = calloc(1, sizeof(vulkan_instance_t));
    if (NULL == vkInstance) {
        fprintf(stderr, "vulkan_create_instance: Failed to allocate memory for VulkanInstance\n");
        return NULL;
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
        return NULL;
    }

    /**
     * Setup the Vulkan Device Object
     */

    /********************************
     * Clean up allocated resources *
     ********************************/

    /** 
     * Destroy the Vulkan Device Object
     */

    /** 
     * Destroy the Vulkan Instance Object
     */
    if (NULL == vkInstance) {
        fprintf(stderr, "vulkan_destroy_instance: vkInstance is NULL\n");
        return;
    }

    if (VK_NULL_HANDLE != vkInstance->handle) {
        vkDestroyInstance(vkInstance->handle, NULL); // Vulkan instance cleanup
    }

    free(vkInstance);

    return 0;
}
