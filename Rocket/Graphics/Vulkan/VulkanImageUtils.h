#pragma once
#include "Vulkan/VulkanVariable.h"
#include "Vulkan/VulkanCheck.h"

namespace Rocket {
    VkResult CreateImage(
        const VkDevice& device, 
        const VkPhysicalDevice& physicalDevice, 
        const VolkDeviceTable& table, 
        uint32_t width, 
        uint32_t height, 
        VkFormat format, 
        VkImageTiling tiling, 
        VkImageUsageFlags usage, 
        VkMemoryPropertyFlags properties, 
        VkImage& image, 
        VkDeviceMemory& imageMemory, 
        VkImageCreateFlags flags = 0, 
        uint32_t mipLevels = 1);

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
    
    VkResult CreateTextureSampler(
        const VkDevice& device, 
        const VkPhysicalDevice& physicalDevice, 
        const VolkDeviceTable& table,
        VkSampler* textureSampler,
        const VkFilter& minFilter = VK_FILTER_LINEAR, 
        const VkFilter& maxFilter = VK_FILTER_LINEAR, 
        const VkSamplerAddressMode& addressMode = VK_SAMPLER_ADDRESS_MODE_REPEAT);

    void TransitionImageLayout(
        VulkanRenderDevice& vkDev, 
        VkImage image, 
        VkFormat format, 
        VkImageLayout oldLayout, 
        VkImageLayout newLayout, 
        uint32_t layerCount = 1, 
        uint32_t mipLevels = 1);

    void TransitionImageLayoutCmd(
        const VolkDeviceTable& table, 
        VkCommandBuffer commandBuffer, 
        VkImage image, 
        VkFormat format, 
        VkImageLayout oldLayout, 
        VkImageLayout newLayout, 
        uint32_t layerCount = 1, 
        uint32_t mipLevels = 1);

    VkFormat FindSupportedFormat(
        const VkPhysicalDevice& device, 
        const std::vector<VkFormat>& candidates, 
        VkImageTiling tiling, 
        VkFormatFeatureFlags features);

    VkFormat FindDepthFormat(const VkPhysicalDevice& device);

    bool HasStencilComponent(VkFormat format);

    VkResult CreateDepthResources(
        VulkanRenderDevice& vkDev, 
        uint32_t width, 
        uint32_t height, 
        VulkanImage& depth);
}
