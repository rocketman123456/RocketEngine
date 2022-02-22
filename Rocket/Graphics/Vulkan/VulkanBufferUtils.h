#pragma once
#include "Vulkan/VulkanVariable.h"
#include "Vulkan/VulkanCheck.h"

namespace Rocket {
    VkResult CreateBuffer(
        const VkDevice& device, 
        const VkPhysicalDevice& physicalDevice, 
        const VolkDeviceTable& table, 
        VkDeviceSize size, 
        VkBufferUsageFlags usage, 
        VkMemoryPropertyFlags properties, 
        VkBuffer& buffer, 
        VkDeviceMemory& bufferMemory);

    VkResult CreateVolume(
        const VkDevice& device, 
        const VkPhysicalDevice& physicalDevice, 
        const VolkDeviceTable& table, 
        uint32_t width, 
        uint32_t height, 
        uint32_t depth,
        VkFormat format, 
        VkImageTiling tiling, 
        VkImageUsageFlags usage, 
        VkMemoryPropertyFlags properties, 
        VkImage& image, 
        VkDeviceMemory& imageMemory, 
        VkImageCreateFlags flags);

    void UploadBufferData(
        const VkDevice& device, 
        const VolkDeviceTable& table, 
        const VkDeviceMemory& bufferMemory, 
        const VkDeviceSize& deviceOffset, 
        const void* data, 
        const size_t dataSize);

    void DownloadBufferData(
        const VkDevice& device, 
        const VolkDeviceTable& table, 
        const VkDeviceMemory& bufferMemory, 
        const VkDeviceSize& deviceOffset, 
        void* outData, 
        const size_t dataSize);
}
