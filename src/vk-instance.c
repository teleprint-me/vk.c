/**
 * Copyright © 2024 Austin Berrio
 *
 * @file src/vk-instance.c
 *
 * @brief Create the VkInstance object with "sane" defaults.
 * 
 * @note Apply zero-initialization strategy to maintain a "sane" default implementation.
 */

#include "vk-instance.h"

#include <vulkan/vulkan.h>
#include <stdio.h>
#include <stdlib.h>

struct VkApplicationInfo* create_vk_application_info(char* pApplicationName, char* pEngineName) {
    struct VkApplicationInfo* applicationInfo = (struct VkApplicationInfo*) malloc(sizeof(struct VkApplicationInfo));
    applicationInfo->sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    applicationInfo->pApplicationName = pApplicationName;
    applicationInfo->applicationVersion = VK_API_VERSION_1_0;
    applicationInfo->pEngineName = pEngineName;
    applicationInfo->engineVersion = VK_API_VERSION_1_0;
    applicationInfo->apiVersion = VK_API_VERSION_1_2;
    return applicationInfo;
}

struct VkInstanceCreateInfo* create_vk_instance_info(struct VkApplicationInfo* pApplicationInfo) {
    struct VkInstanceCreateInfo* instanceInfo = (struct VkInstanceCreateInfo*) malloc(sizeof(struct VkInstanceCreateInfo));
    instanceInfo->sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceInfo->pApplicationInfo = pApplicationInfo;
    instanceInfo->enabledExtensionCount = 0;
    instanceInfo->ppEnabledExtensionNames = NULL;
    instanceInfo->enabledLayerCount = 0;
    instanceInfo->ppEnabledLayerNames = NULL;
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

vulkan_instance_t* create_vulkan_instance(char* pApplicationName, char* pEngineName) {
    vulkan_instance_t* vkInstance = (vulkan_instance_t*) malloc(sizeof(vulkan_instance_t));

    vkInstance->applicationInfo = create_vk_application_info(pApplicationName, pEngineName);
    vkInstance->instanceCreateInfo = create_vk_instance_info(&vkInstance->applicationInfo);
    
    VkResult result = vkCreateInstance(vkInstance->instanceCreateInfo, NULL, vkInstance->handle);
    if (VK_SUCCESS != result) {
        fprintf(stderr, "Failed to create Vulkan instance! (Error code: %d)\n", result);
        exit(EXIT_FAILURE);
    }
    
    return vkInstance;
}

void destroy_vulkan_instance(vulkan_instance_t* vkInstance) {
    if (vkInstance) {
        vkDestroyInstance(vkInstance->handle, NULL); // Vulkan instance cleanup
        free(vkInstance->applicationInfo); // Free application info
        free(vkInstance->instanceCreateInfo); // Free instance info
        free(vkInstance); // Finally, free the vulkanInstance itself
    }
}
