/**
 * Copyright © 2024 Austin Berrio
 *
 * @file include/vk-shader.h
 */

#ifndef VK_SHADER_H
#define VK_SHADER_H

#include <vulkan/vulkan.h>
#include <stdio.h>

// @brief Creates and returns a VkShaderModuleCreateInfo structure with default values.
VkShaderModuleCreateInfo vulkan_create_shader_module_info(void);

// @brief Reads the shader module file and returns the FILE*.
FILE* vulkan_read_shader_module(const char* filepath);

// @brief Calculates the size of the shader module file.
size_t vulkan_calculate_shader_module_size(FILE* file);

// @brief Creates a buffer for the shader module based on the file size.
char* vulkan_create_shader_module_buffer(FILE* file, size_t fileSize);

// @brief Sets up the VkShaderModuleCreateInfo structure with the buffer and file size.
void vulkan_setup_shader_module_info(
    VkShaderModuleCreateInfo* shaderInfo, char* buffer, size_t fileSize
);

// @brief Creates the shader module using the device and shader module info.
VkShaderModule vulkan_create_shader_module(
    VkDevice device, VkShaderModuleCreateInfo* shaderInfo, const char* filepath
);

#endif // VK_SHADER_H
