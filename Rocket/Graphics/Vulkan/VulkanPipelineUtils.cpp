#include "Vulkan/VulkanPipelineUtils.h"

namespace Rocket {
    VkPipelineShaderStageCreateInfo ShaderStageInfo(
        VkShaderStageFlagBits shaderStage, 
        const VulkanShaderModule& module, 
        const char* entryPoint
    ) {
        VkPipelineShaderStageCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        createInfo.pNext = nullptr;
        createInfo.flags = 0;
        createInfo.stage = shaderStage;
        createInfo.module = module.shader_module;
        createInfo.pName = entryPoint;
        createInfo.pSpecializationInfo = nullptr;
        return createInfo;
    }

    VkDescriptorSetLayoutBinding DescriptorSetLayoutBinding(
        uint32_t binding, 
        VkDescriptorType descriptorType, 
        VkShaderStageFlags stageFlags, 
        uint32_t descriptorCount
    ) {
        VkDescriptorSetLayoutBinding layout = {};
        layout.binding = binding;
        layout.descriptorType = descriptorType;
        layout.descriptorCount = descriptorCount;
        layout.stageFlags = stageFlags;
        layout.pImmutableSamplers = nullptr;
        return layout;
    }

    VkWriteDescriptorSet BufferWriteDescriptorSet(
        VkDescriptorSet ds, 
        const VkDescriptorBufferInfo* bi, 
        uint32_t bindIdx, 
        VkDescriptorType dType
    ) {
        VkWriteDescriptorSet descriptorSet = {};
        descriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorSet.pNext = nullptr;
        descriptorSet.dstSet = ds;
        descriptorSet.dstBinding = bindIdx;
        descriptorSet.dstArrayElement = 0;
        descriptorSet.descriptorCount = 1; 
        descriptorSet.descriptorType = dType; 
        descriptorSet.pImageInfo = nullptr;
        descriptorSet.pBufferInfo = bi;
        descriptorSet.pTexelBufferView = nullptr;
        return descriptorSet;
    }

    VkWriteDescriptorSet ImageWriteDescriptorSet(
        VkDescriptorSet ds, 
        const VkDescriptorImageInfo* ii, 
        uint32_t bindIdx
    ) {
        VkWriteDescriptorSet descriptorSet = {};
        descriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorSet.pNext = nullptr;
        descriptorSet.dstSet = ds;
        descriptorSet.dstBinding = bindIdx;
        descriptorSet.dstArrayElement = 0;
        descriptorSet.descriptorCount = 1; 
        descriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER; 
        descriptorSet.pImageInfo = ii;
        descriptorSet.pBufferInfo = nullptr;
        descriptorSet.pTexelBufferView = nullptr;
        return descriptorSet;
    }
}
