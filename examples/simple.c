/**
 * @file examples/simple.c
 */

#include <vulkan/vulkan.h>

#include <stdio.h>
#include <stdlib.h>

// ***
// Create the Vulkan instance object
// ***

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

struct VkInstance_T* create_vk_instance(VkInstanceCreateInfo* pInstanceInfo) {
    struct VkInstance_T* pVkInstance;

    if (VK_SUCCESS != vkCreateInstance(pInstanceInfo, NULL, &pVkInstance)) {
        fprintf(stderr, "Failed to create Vulkan instance!\n");
        exit(EXIT_FAILURE);
    }

    return pVkInstance;
}

// ***
// Create the Vulkan device object
// ***

VkDeviceQueueCreateInfo create_device_queue_info(void) {
    static float queuePriority = 1.0f; // Default priority
    VkDeviceQueueCreateInfo queueCreateInfo = {};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = 0; // Default to the first queue family
    queueCreateInfo.queueCount = 1; // Start with one queue for now
    queueCreateInfo.pQueuePriorities = &queuePriority; // Required field
    return queueCreateInfo;
}

uint32_t get_physical_device_count(struct VkInstance_T* pVkInstance) {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(pVkInstance, &deviceCount, NULL);

    if (0 == deviceCount) {
        fprintf(stderr, "Failed to find GPUs with Vulkan support!\n");
        exit(EXIT_FAILURE);
    }

    return deviceCount;
}

struct VkPhysicalDevice_T* create_physical_devices(struct VkInstance_T* pVkInstance, uint32_t deviceCount) {
    struct VkPhysicalDevice_T* pPhysicalDevices = malloc(deviceCount * sizeof(struct VkPhysicalDevice_T*));
    vkEnumeratePhysicalDevices(pVkInstance, &deviceCount, pPhysicalDevices);
    return pPhysicalDevices;
}

void destroy_physical_devices(struct VkPhysicalDevice_T* pPhysicalDevices) {
    if (pPhysicalDevices) {
        free(pPhysicalDevices);
    }
}

VkPhysicalDeviceProperties
get_physical_device_properties(struct VkPhysicalDevice_T* pPhysicalDevice, uint32_t deviceCount) {
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(pPhysicalDevice, &deviceProperties);
    return deviceProperties;
}

// attempt to guess which device should be returned
VkPhysicalDevice get_physical_device(
    struct VkInstance_T* pVkInstance, struct VkPhysicalDevice_T* pPhysicalDevices, uint32_t deviceCount
) {
    for (uint32_t i = 0; i < deviceCount; i++) {
        VkPhysicalDeviceProperties deviceProperties
            = get_physical_device_properties((&pPhysicalDevices)[i], deviceCount);
        // If device supports compute (discrete GPU preferred)
        if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
            return (&pPhysicalDevices)[i];
        }
    }

    // Fallback to first available device
    return (&pPhysicalDevices)[0];
}

// @note we only need a help text and the ability to pass a file path to the
// shader being utilized for compute operations.
int main(int argc, char* argv[]) {
    // Create the vulkan instance objects
    VkApplicationInfo applicationInfo = create_vk_application_info("SimpleApp", "SimpleEngine");
    VkInstanceCreateInfo instanceInfo = create_vk_instance_info(&applicationInfo);
    VkInstance vkInstance = create_vk_instance(&instanceInfo);

    // Create the vulkan device instance objects
    uint32_t deviceCount = get_physical_device_count(vkInstance);
    struct VkPhysicalDevice_T* pPhysicalDevices = create_physical_devices(vkInstance, deviceCount);

    VkDeviceQueueCreateInfo deviceQueueInfo = create_device_queue_info();
    VkDevice vkDevice = create_vk_device();

    // Clean up
    vkDestroyInstance(vkInstance, NULL);
    vkDestroyDevice(vkDevice, NULL);

    return 0;
}
