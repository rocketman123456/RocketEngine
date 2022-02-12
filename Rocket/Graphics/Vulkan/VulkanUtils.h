#pragma once
#include "Vulkan/VulkanVariable.h"
#include "Vulkan/VulkanConstant.h"
#include "Vulkan/VulkanCheck.h"

#include <volk.h>

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

    VkResult CreateImageView(
        const VkDevice& device, 
        const VolkDeviceTable& table,
        const VkImage& image, 
        const VkFormat& format, 
        const VkImageAspectFlags& aspectFlags, 
        VkImageView* imageView, 
        // Optional
        VkImageViewType viewType = VK_IMAGE_VIEW_TYPE_2D, 
        uint32_t layerCount = 1, 
        uint32_t mipLevels = 1);

    VkResult CreateSemaphore(
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
