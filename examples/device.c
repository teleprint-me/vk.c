/**
 * Copyright © 2024 Austin Berrio
 *
 * @file examples/device.c
 * 
 * @brief A simple example showcasing how to create and destroy custom vulkan instance, device, and queue objects.
 */

#include "vk-instance.h"
#include "vk-device.h"

int main(void) {
    vulkan_instance_t* vkInstance = create_vulkan_instance(
        "DeviceApp", "DeviceEngine"
    );
    vulkan_device_t* vkDevice = create_vulkan_device(vkInstance);
    vulkan_queue_t* vkQueue = create_vulkan_queue(vkDevice->physical);

    destroy_vulkan_queue(vkQueue);
    destroy_vulkan_device(vkDevice);
    destroy_vulkan_instance(vkInstance);

    return 0;
}
