#pragma once
#include "Vulkan/VulkanVariable.h"
#include "Vulkan/VulkanConstant.h"
#include "Vulkan/VulkanCheck.h"

#include <volk.h>

namespace Rocket {
//     // Buffer Related
//     size_t AllocateVertexBuffer(
//         const VkDevice& device, 
//         const VolkDeviceTable& table, 
//         const VkPhysicalDevice& physicalDevice,
//         const VkCommandPool& commandPool,
//         const VkQueue& graphicsQueue,
//         VkBuffer* storageBuffer, 
//         VkDeviceMemory* storageBufferMemory, 
//         size_t vertexDataSize, 
//         const void* vertexData, 
//         size_t indexDataSize, 
//         const void* indexData);
//     bool CreateBuffer(
//         const VkDevice& device, 
//         const VolkDeviceTable& table, 
//         const VkPhysicalDevice& physicalDevice,
//         const VkDeviceSize& size, 
//         const VkBufferUsageFlags& usage, 
//         const VkMemoryPropertyFlags& properties, 
//         VkBuffer* buffer,
//         VkDeviceMemory* bufferMemory);
//     void CopyBuffer(
//         const VkDevice& device, 
//         const VolkDeviceTable& table, 
//         const VkPhysicalDevice& physicalDevice,
//         const VkCommandPool& commandPool,
//         const VkQueue& graphicsQueue,
//         const VkDeviceSize& size,
//         VkBuffer* srcBuffer, 
//         VkBuffer* dstBuffer);
//     VkCommandBuffer BeginSingleTimeCommands(
//         const VkDevice& device, 
//         const VolkDeviceTable& table,
//         const VkCommandPool& commandPool);
//     void EndSingleTimeCommands(
//         const VkDevice& device, 
//         const VolkDeviceTable& table,
//         const VkQueue& graphicsQueue,
//         const VkCommandPool& commandPool,
//         const VkCommandBuffer& commandBuffer);
//     bool CreateSharedBuffer(
//         const VkDevice& device, 
//         const VolkDeviceTable& table, 
//         const VkPhysicalDevice& physicalDevice,
//         const VkDeviceSize size, 
//         const VkBufferUsageFlags usage, 
//         const VkMemoryPropertyFlags properties, 
//         QueueFamilyIndices& indices,
//         VkBuffer* buffer, 
//         VkDeviceMemory* bufferMemory);
//     bool CreateUniformBuffer(
//         const VkDevice& device, 
//         const VolkDeviceTable& table, 
//         const VkPhysicalDevice& physicalDevice,
//         const VkDeviceSize& bufferSize,
//         VkBuffer* buffer, 
//         VkDeviceMemory* bufferMemory);
//     void UploadBufferData(
//         const VkDevice& device, 
//         const VolkDeviceTable& table, 
//         const VkDeviceMemory& bufferMemory, 
//         VkDeviceSize deviceOffset, 
//         const void* data, 
//         const size_t dataSize);
//     void DownloadBufferData(
//         const VkDevice& device, 
//         const VolkDeviceTable& table, 
//         const VkDeviceMemory& bufferMemory, 
//         VkDeviceSize deviceOffset, 
//         void* outData, 
//         const size_t dataSize);
}
