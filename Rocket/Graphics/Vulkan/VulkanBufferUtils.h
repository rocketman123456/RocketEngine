#pragma once
#include "Vulkan/VulkanVariable.h"
#include "Vulkan/VulkanCheck.h"

namespace Rocket {
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
