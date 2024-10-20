/**
 * Copyright © 2024 Austin Berrio
 *
 * @file src/vk-shader.c
 */

#include "vk-shader.h"

#include <stdlib.h>

FILE* read_shader_module(const char* filepath) {
    FILE* file = fopen(filepath, "rb");
    if (!file) {
        fprintf(stderr, "Failed to open SPIR-V file: %s\n", filepath);
        exit(EXIT_FAILURE);
    }
    return file;
}

size_t calculate_shader_module_size(FILE* file) {
    // Get file size
    fseek(file, 0, SEEK_END);
    size_t fileSize = ftell(file);
    rewind(file);
    return fileSize;
}

char* create_shader_module_buffer(FILE* file, size_t fileSize) {
    char* buffer = (char*)malloc(fileSize);
    if (!buffer) {
        fprintf(stderr, "Failed to allocate shader module buffer: %zu\n", fileSize);
        exit(EXIT_FAILURE);
    }
    fread(buffer, 1, fileSize, file);
    return buffer;
}

void setup_shader_module_info(
    VkShaderModuleCreateInfo* shaderInfo, char* buffer, size_t fileSize
) {
    shaderInfo->codeSize = fileSize;
    shaderInfo->pCode = (uint32_t*)buffer;
}

VkShaderModule create_shader_module(
    VkDevice device, VkShaderModuleCreateInfo* shaderInfo, const char* filepath
) {
    FILE* file = read_shader_module(filepath);
    size_t fileSize = calculate_shader_module_size(file);
    char* buffer = create_shader_module_buffer(file, fileSize);
    setup_shader_module_info(shaderInfo, buffer, fileSize);

    VkShaderModule shaderModule;
    if (VK_SUCCESS != vkCreateShaderModule(device, shaderInfo, NULL, &shaderModule)) {
        fprintf(stderr, "Failed to create shader module!\n");
        exit(EXIT_FAILURE);
    }

    // clean up
    fclose(file); // close the shader module binary file
    free(buffer); // close the shader module memory buffer
    return shaderModule;
}
