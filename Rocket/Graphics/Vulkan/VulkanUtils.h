#pragma once
#include "Vulkan/VulkanVariable.h"
#include "Vulkan/VulkanConstant.h"
#include "Vulkan/VulkanCheck.h"

#include <volk.h>
#include <string>

namespace Rocket {
    uint32_t GetVulkanBufferAlignment(VulkanRenderDevice& vkDev);

    bool IsDepthFormat(VkFormat fmt);

    bool SetVkObjectName(
        VulkanRenderDevice& vkDev, 
        void* object, 
        VkObjectType objType, 
        const std::string& name);

    bool SetVkImageName(
        VulkanRenderDevice& vkDev, 
        void* object, 
        const std::string& name);

    VkResult CreateSemaphoreVk(
        const VkDevice& device, 
        const VolkDeviceTable& table,
        VkSemaphore* outSemaphore);

    VkResult CreateFence(
        const VkDevice& device, 
        const VolkDeviceTable& table,
        VkFence* fence);

    VkResult CreateCommandPool(
        const VkDevice& device, 
        const VolkDeviceTable& table, 
        const QueueFamilyIndices& indices, 
        VkCommandPool* command_pool);

    VkResult CreateCommandBuffer(
        const VkDevice& device, 
        const VolkDeviceTable& table,
        const VkCommandPool& commandPool,
        uint32_t imageCount,
        VkCommandBuffer* command_buffer);
}
