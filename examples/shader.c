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

#include <getopt.h>
#include <unistd.h>
#include <stdbool.h>

void print_usage(char* argv[]) {
    fprintf(
        stderr,
        "Usage: "
        "%s [--help] "
        "path/to/file.comp\n",
        argv[0]
    );
}

// Define command line option structure
static struct option long_options[]
    = {{"help", 0, NULL, 'h'},
       {0, 0, NULL, 0}};

int main(int argc, char* argv[]) {
    const char* appName = "DeviceApp";
    const char* engineName = "DeviceEngine";
    char* shaderPath = NULL;  // Path to SPIR-V shader file

    while (true) {
        static int opt_index = 0;

        int c = getopt_long(argc, argv, "h:s:n:m:", long_options, &opt_index);

        if (c == -1) {
            break;
        }

        switch (c) {
            case 'h':
                print_usage(argv);
                return EXIT_SUCCESS;
            case 's':
                // Convert seed input
                shaderPath = (char*) optarg;
                if (NULL == shaderPath) {
                    fprintf(stderr, "Failed to create GLSL shader path.\n");
                    return EXIT_FAILURE;
                }
                printf("Setting position to: %d\n", shaderPath);
                break;
            default:
                print_usage(argv);
                return EXIT_FAILURE;
        }
    }

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
