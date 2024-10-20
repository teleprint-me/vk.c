/**
 * Copyright © 2024 Austin Berrio
 *
 * @file include/vk-shader.h
 */

#ifndef VK_SHADER_H
#define VK_SHADER_H

#include <vulkan/vulkan.h>
#include <stdio.h>

/**
 * @brief Structure representing a Vulkan shader and related data.
 */
typedef struct VulkanShader {
    VkShaderModule module;  ///< Handle to the Vulkan shader module
    char* buffer;           ///< Buffer containing the shader code
    size_t bufferSize;      ///< Size of the shader code buffer
} vulkan_shader_t;

// @brief Creates and returns a VkShaderModuleCreateInfo structure with default values.
VkShaderModuleCreateInfo vulkan_create_shader_module_info(void);

// @brief Reads the shader module file and returns the buffer.
FILE* vulkan_read_shader_module(const char* filepath);

// @brief Calculates the size of the shader module file.
size_t vulkan_calculate_shader_module_size(FILE* file);

// @brief Creates the shader module buffer based on the file size.
char* vulkan_create_shader_module_buffer(FILE* file, size_t fileSize);

// @brief Sets up the VkShaderModuleCreateInfo structure with the buffer and file size.
void vulkan_setup_shader_module_info(
    VkShaderModuleCreateInfo* shaderInfo, char* buffer, size_t fileSize
);

// @brief Initializes the vulkan_shader_t structure and creates the shader module.
vulkan_shader_t* vulkan_create_shader(VkDevice device, const char* filepath);

// @brief Destroys the shader module and frees related resources.
void vulkan_destroy_shader(VkDevice device, vulkan_shader_t* shader);

#endif // VK_SHADER_H
