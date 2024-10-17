/**
 * @file src/vk-instance.c
 * 
 * @note Apply zero-initialization strategy to maintain a "sane" default implementation.
 */

#include <vulkan/vulkan.h>

#include <stdio.h>
#include <stdlib.h>

// ***
// Create the Vulkan instance object
// ***

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
    // No extensions by default
    instanceInfo.enabledExtensionCount = 0;
    instanceInfo.ppEnabledExtensionNames = NULL;
    // No validation layers by default
    instanceInfo.enabledLayerCount = 0;
    instanceInfo.ppEnabledLayerNames = NULL;
    return instanceInfo;
}

// dedicated function for adding extensions to an existing VkInstanceCreateInfo object instance

// dedicated function for adding validation layers to an existing VkInstanceInfo object instance

struct VkInstance_T* create_vk_instance(struct VkInstanceCreateInfo* pInstanceInfo) {
    struct VkInstance_T* pVkInstance;

    enum VkResult result = vkCreateInstance(pInstanceInfo, NULL, &pVkInstance);
    if (VK_SUCCESS != result) {
        fprintf(stderr, "Failed to create Vulkan instance!\n");
        exit(EXIT_FAILURE);
    }

    return pVkInstance;
}
