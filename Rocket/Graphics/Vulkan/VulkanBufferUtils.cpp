#include "Vulkan/VulkanBufferUtils.h"

namespace Rocket {
    void UploadBufferData(
        const VkDevice& device, 
        const VolkDeviceTable& table, 
        const VkDeviceMemory& bufferMemory, 
        const VkDeviceSize& deviceOffset, 
        const void* data, 
        const size_t dataSize
    ) {
        void* mappedData = nullptr;
        table.vkMapMemory(device, bufferMemory, deviceOffset, dataSize, 0, &mappedData);
            memcpy(mappedData, data, dataSize);
        table.vkUnmapMemory(device, bufferMemory);
    }

    void DownloadBufferData(
        const VkDevice& device, 
        const VolkDeviceTable& table, 
        const VkDeviceMemory& bufferMemory, 
        const VkDeviceSize& deviceOffset, 
        void* outData, 
        const size_t dataSize
    ) {
        void* mappedData = nullptr;
        table.vkMapMemory(device, bufferMemory, deviceOffset, dataSize, 0, &mappedData);
            memcpy(outData, mappedData, dataSize);
        table.vkUnmapMemory(device, bufferMemory);
    }
}
