/**
 * Copyright © 2024 Austin Berrio
 *
 * @file examples/instance.c
 * 
 * @brief Simple example showcasing how to create and destroy a custom vulkan instance object.
 */

#include "vk-instance.h"

int main(void) {
    vulkan_instance_t* vkInstance = create_vulkan_instance("InstanceApp", "InstanceEngine");
    printf("Successfully Created vulkan instance!\n");

    destroy_vulkan_instance(vkInstance);
    printf("Successfully destroyed vulkan instance!\n");

    return 0;
}
