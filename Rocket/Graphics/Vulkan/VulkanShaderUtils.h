#pragma once
#include "Vulkan/VulkanVariable.h"
#include "Vulkan/VulkanCheck.h"

#include <shaderc/shaderc.hpp>

namespace Rocket {
    VkShaderStageFlagBits ShaderStageToVulkan(shaderc_shader_kind sh);
    shaderc_shader_kind ShaderStageFromFileName(const std::string& fileName);

    std::string PreProcessShader(
        shaderc_shader_kind kind,
        const std::string& source_name,
        const std::string& source);
    
    std::string CompileToAssembly(
        shaderc_shader_kind kind,
        const std::string& source_name,
        const std::string& source,
        bool optimize = false);

    std::vector<uint32_t> CompileToBinary(
        shaderc_shader_kind kind,
        const std::string& source_name,
        const std::string& source,
        bool optimize = false);

    std::size_t CompileShaderFile(
        const std::string& root, 
        const std::string& file, 
        VulkanShaderModule* module);

    VkResult CreateShaderModule(
        const VkDevice& device, 
        const VolkDeviceTable& table, 
        const std::string& root, 
        const std::string& fileName,
        VulkanShaderModule* shader);
}
