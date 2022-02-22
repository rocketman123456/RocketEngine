#include "Vulkan/VulkanBufferUtils.h"
#include "Vulkan/VulkanDeviceUtils.h"

namespace Rocket {
    VkResult CreateBuffer(
        const VkDevice& device, 
        const VkPhysicalDevice& physicalDevice, 
        const VolkDeviceTable& table, 
        VkDeviceSize size, 
        VkBufferUsageFlags usage, 
        VkMemoryPropertyFlags properties, 
        VkBuffer& buffer, 
        VkDeviceMemory& bufferMemory
    ) {
        VkBufferCreateInfo bufferInfo = {};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.pNext = nullptr;
        bufferInfo.flags = 0;
        bufferInfo.size = size;
        bufferInfo.usage = usage;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        bufferInfo.queueFamilyIndexCount = 0;
        bufferInfo.pQueueFamilyIndices = nullptr;

        VK_CHECK(table.vkCreateBuffer(device, &bufferInfo, nullptr, &buffer));

        VkMemoryRequirements memRequirements;
        table.vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

        VkMemoryAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.pNext = nullptr;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = FindMemoryType(physicalDevice, memRequirements.memoryTypeBits, properties);

        VK_CHECK(table.vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory));

        vkBindBufferMemory(device, buffer, bufferMemory, 0);

        return VK_SUCCESS;
    }

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
        VkImageCreateFlags flags
    ) {
        VkImageCreateInfo imageInfo = {};
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.pNext = nullptr;
        imageInfo.flags = flags;
        imageInfo.imageType = VK_IMAGE_TYPE_3D;
        imageInfo.format = format;
        imageInfo.extent.width = width;
        imageInfo.extent.height = height;
        imageInfo.extent.depth = depth;
        imageInfo.mipLevels = 1;
        imageInfo.arrayLayers = 1;
        imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        imageInfo.tiling = tiling;
        imageInfo.usage = usage;
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        imageInfo.queueFamilyIndexCount = 0;
        imageInfo.pQueueFamilyIndices = nullptr;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

        VK_CHECK(table.vkCreateImage(device, &imageInfo, nullptr, &image));

        VkMemoryRequirements memRequirements;
        vkGetImageMemoryRequirements(device, image, &memRequirements);

        VkMemoryAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.pNext = nullptr;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = FindMemoryType(physicalDevice, memRequirements.memoryTypeBits, properties);

        VK_CHECK(table.vkAllocateMemory(device, &allocInfo, nullptr, &imageMemory));

        vkBindImageMemory(device, image, imageMemory, 0);
        return VK_SUCCESS;
    }

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
