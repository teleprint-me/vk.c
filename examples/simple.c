/**
 * @file examples/simple.c
 */

#include <vulkan/vulkan.h>

#include <stdio.h>
#include <stdlib.h>

// @note Apply zero-initialization strategy to maintain a "sane"
// implementation.
VkApplicationInfo create_vk_application_info(char* pApplicationName, char* pEngineName) {
    VkApplicationInfo application_info = {};
    application_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    application_info.pApplicationName = pApplicationName;
    application_info.applicationVersion = VK_API_VERSION_1_0;
    application_info.pEngineName = pEngineName;
    application_info.engineVersion = VK_API_VERSION_1_0;
    application_info.apiVersion = VK_API_VERSION_1_2;
    return application_info;
}

VkInstanceCreateInfo create_vk_instance_info(VkApplicationInfo* pApplicationInfo) {
    VkInstanceCreateInfo instance_info = {};
    instance_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instance_info.pApplicationInfo = pApplicationInfo;
    // No extensions by default
    instance_info.enabledExtensionCount = 0;
    instance_info.ppEnabledExtensionNames = NULL;
    // No validation layers by default
    instance_info.enabledLayerCount = 0;
    instance_info.ppEnabledLayerNames = NULL;
    return instance_info;
}

VkInstance create_vk_instance(VkInstanceCreateInfo* pInstanceInfo) {
    VkInstance vk_instance;

    if (vkCreateInstance(pInstanceInfo, NULL, &vk_instance) != VK_SUCCESS) {
        fprintf(stderr, "Failed to create Vulkan instance!\n");
        exit(EXIT_FAILURE);
    }

    return vk_instance;
}

// @note we only need a help text and the ability to pass a file path to the
// shader being utilized for compute operations.
int main(int argc, char* argv[]) {
    // Initialize the vulkan instance object
    VkApplicationInfo applicationInfo = create_vk_application_info("SimpleApp", "SimpleEngine");
    VkInstanceCreateInfo instanceInfo = create_vk_instance_info(&applicationInfo);
    VkInstance vkInstance = create_vk_instance(&instanceInfo);

    vkDestroyInstance(vkInstance, NULL);
    return 0;
}
