/**
 * Copyright © 2024 Austin Berrio
 *
 * @file include/vk-shader.h
 */

#ifndef VK_SHADER
#define VK_SHADER

#include <vulkan/vulkan.h>
#include <stdio.h>

FILE* read_shader_module(const char* filepath);
size_t calculate_shader_module_size(FILE* file);
char* create_shader_module_buffer(FILE* file, size_t fileSize);
void setup_shader_module_info(
    VkShaderModuleCreateInfo* shaderInfo, char* buffer, size_t fileSize
);
VkShaderModule create_shader_module(
    VkDevice device, VkShaderModuleCreateInfo* shaderInfo, const char* filepath
);

#endif // VK_SHADER
