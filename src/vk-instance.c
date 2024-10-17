/**
 * Copyright © 2024 Austin Berrio
 *
 * @file src/vk-instance.c
 *
 * @brief Create the VkInstance object with "sane" defaults.
 * 
 * @note Apply zero-initialization strategy to maintain a "sane" default implementation.
 */

#include <vulkan/vulkan.h>
#include <stdio.h>
#include <stdlib.h>

struct VkApplicationInfo create_vk_application_info(char* pApplicationName, char* pEngineName) {
    struct VkApplicationInfo applicationInfo = {};
    applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    applicationInfo.pApplicationName = pApplicationName;
    applicationInfo.applicationVersion = VK_API_VERSION_1_0;
    applicationInfo.pEngineName = pEngineName;
    applicationInfo.engineVersion = VK_API_VERSION_1_0;
    applicationInfo.apiVersion = VK_API_VERSION_1_2;
    return applicationInfo;
}

struct VkInstanceCreateInfo create_vk_instance_info(struct VkApplicationInfo* pApplicationInfo) {
    struct VkInstanceCreateInfo instanceInfo = {};
    instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceInfo.pApplicationInfo = pApplicationInfo;
    instanceInfo.enabledExtensionCount = 0;
    instanceInfo.ppEnabledExtensionNames = NULL;
    instanceInfo.enabledLayerCount = 0;
    instanceInfo.ppEnabledLayerNames = NULL;
    return instanceInfo;
}

void set_instance_info_extensions(struct VkInstanceCreateInfo* pInstanceInfo, const char** extensions, uint32_t extensionCount) {
    pInstanceInfo->enabledExtensionCount = extensionCount;
    pInstanceInfo->ppEnabledExtensionNames = extensions;
}

void set_instance_info_validation_layers(struct VkInstanceCreateInfo* pInstanceInfo, const char** layers, uint32_t layerCount) {
    pInstanceInfo->enabledLayerCount = layerCount;
    pInstanceInfo->ppEnabledLayerNames = layers;
}

struct VkInstance_T* create_vk_instance(struct VkInstanceCreateInfo* pInstanceInfo) {
    struct VkInstance_T* pVkInstance;
    enum VkResult result = vkCreateInstance(pInstanceInfo, NULL, &pVkInstance);
    if (VK_SUCCESS != result) {
        fprintf(stderr, "Failed to create Vulkan instance! (Error code: %d)\n", result);
        exit(EXIT_FAILURE);
    }
    return pVkInstance;
}
