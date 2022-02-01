#pragma once
#include "Vulkan/VulkanVariable.h"
#include "Vulkan/VulkanConstant.h"
#include "Vulkan/VulkanCheck.h"

namespace Rocket {
    VkPipelineShaderStageCreateInfo ShaderStageInfo(
        VkShaderStageFlagBits shaderStage, 
        const VulkanShaderModule& module, 
        const char* entryPoint);

    VkDescriptorSetLayoutBinding DescriptorSetLayoutBinding(
        uint32_t binding, 
        VkDescriptorType descriptorType, 
        VkShaderStageFlags stageFlags, 
        uint32_t descriptorCount = 1);
    
    VkWriteDescriptorSet BufferWriteDescriptorSet(
        VkDescriptorSet ds, 
        const VkDescriptorBufferInfo* bi, 
        uint32_t bindIdx, 
        VkDescriptorType dType);
    
    VkWriteDescriptorSet ImageWriteDescriptorSet(
        VkDescriptorSet ds, 
        const VkDescriptorImageInfo* ii, 
        uint32_t bindIdx);
}
