/**
 * Copyright © 2024 Austin Berrio
 *
 * @file examples/shader.c
 * 
 * @brief A simple example showcasing how to create and destroy custom
 * vulkan instance, device and queue, and shader objects.
 */

#include "vk-instance.h"
#include "vk-device.h"
#include "vk-shader.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    const char* appName = "DeviceApp";
    const char* engineName = "DeviceEngine";
    const char* shaderPath = "path/to/shader.spv";  // Path to your SPIR-V shader file
    
    // Create the instance object
    vulkan_instance_t* vkInstance = vulkan_create_instance(appName, engineName);
    if (NULL == vkInstance) {
        fprintf(stderr, "Failed to create Vulkan instance.\n");
        return EXIT_FAILURE;
    }
    printf("Successfully created Vulkan instance!\n");

    // Create the device object
    vulkan_device_t* vkDevice = vulkan_create_device(vkInstance->handle);
    if (NULL == vkDevice) {
        fprintf(stderr, "Failed to create Vulkan device.\n");
        return EXIT_FAILURE;
    }
    printf("Successfully created Vulkan device!\n");

    // Create the shader object
    vulkan_shader_t* vkShader = vulkan_create_shader(vkDevice->logicalDevice, shaderPath);
    if (NULL == vkShader) {
        fprintf(stderr, "Failed to create Vulkan shader.\n");
        return EXIT_FAILURE;
    }
    printf("Successfully created Vulkan shader!\n");

    // Destroy the shader object
    vulkan_destroy_shader(vkDevice->logicalDevice, vkShader);
    printf("Successfully destroyed Vulkan shader!\n");

    // Destroy the device object
    vulkan_destroy_device(vkDevice);
    printf("Successfully destroyed Vulkan device!\n");

    // Destroy the instance object
    vulkan_destroy_instance(vkInstance);
    printf("Successfully destroyed Vulkan instance!\n");

    return EXIT_SUCCESS;
}
