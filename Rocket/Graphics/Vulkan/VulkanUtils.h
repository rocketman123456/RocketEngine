#pragma once
#include "Vulkan/VulkanVariable.h"

#include <volk.h>

namespace Rocket {
    // Create Instace
    VkInstance CreateVulkanInstance(
        const std::vector<const char*>& validationLayers,
        const std::vector<const char*>& instanceExtension);
    // Print Version
    void PrintVulkanVersion();
    // Setup Debug Messenger
    VkDebugUtilsMessengerEXT SetupDebugMessenger(
        const VkInstance& instance);
    VkDebugReportCallbackEXT SetupDebugReportCallback(
        const VkInstance& instance);
    // Pick Physical Device
    VkPhysicalDevice PickPhysicalDevice(
        const VkInstance& instance, 
        const VkSurfaceKHR& surface, 
        const std::vector<const char*>& deviceExtensions);
    // Create Logical Device
    VkDevice CreateLogicalDevice(
        const VkPhysicalDevice& device, 
        const std::vector<const char*>& deviceExtensions, 
        const std::vector<const char*>& validationLayers,
        const QueueFamilyIndices& indices);
    uint32_t FindQueueFamilies(
        const VkPhysicalDevice& device, 
        const VkQueueFlags& desiredFlags);
    uint32_t FindPresentFamilies(
        const VkPhysicalDevice& device, 
        const VkSurfaceKHR& surface);
    QueueFamilyIndices FindQueueFamilies(
        const VkPhysicalDevice& device,
        const VkSurfaceKHR& surface);
    // Create Swap Chain
    VkSwapchainKHR CreateSwapChain(
        const VkPhysicalDevice& physicalDevice, 
        const VkDevice& device,
        const VolkDeviceTable& table,
        const VkSurfaceKHR& surface, 
        const QueueFamilyIndices& indices, 
        VkFormat* swapChainImageFormat,
        uint32_t width, 
        uint32_t height, 
        bool supportScreenshots);
    // Create Swap Chain Image View
    size_t CreateSwapchainImages(
        const VkDevice& device, 
        const VolkDeviceTable& table, 
        const VkSwapchainKHR& swapchain, 
        const VkFormat& swapChainImageFormat,
        std::vector<VkImage>& swapchainImages,
        std::vector<VkImageView>& swapchainImageViews);
    void CreateImageView(
        const VkDevice& device, 
        const VolkDeviceTable& table, 
        const VkImage& image, 
        const VkFormat& format, 
        const VkImageAspectFlags& aspectFlags, 
        VkImageView* imageView, 
        const VkImageViewType& viewType = VK_IMAGE_VIEW_TYPE_2D,
        uint32_t layerCount = 1, 
        uint32_t mipLevels = 1);
    // Create Semaphore
    void CreateVulkanSemaphore(
        const VkDevice& device, 
        const VolkDeviceTable& table, 
        VkSemaphore* outSemaphore);
    // Create Command Pool
    void CreateCommandPool(
        const VkDevice& device, 
        const VolkDeviceTable& table, 
        const QueueFamilyIndices& indices, 
        VkCommandPool* command_pool);
    void CreateCommandBuffer(
        const VkDevice& device, 
        const VolkDeviceTable& table,
        const VkCommandPool& commandPool,
        uint32_t imageCount,
        VkCommandBuffer* command_buffer);
    // Buffer Related
    size_t AllocateVertexBuffer(
        const VkDevice& device, 
        const VolkDeviceTable& table, 
        const VkPhysicalDevice& physicalDevice,
        const VkCommandPool& commandPool,
        const VkQueue& graphicsQueue,
        VkBuffer* storageBuffer, 
        VkDeviceMemory* storageBufferMemory, 
        size_t vertexDataSize, 
        const void* vertexData, 
        size_t indexDataSize, 
        const void* indexData);
    bool CreateBuffer(
        const VkDevice& device, 
        const VolkDeviceTable& table, 
        const VkPhysicalDevice& physicalDevice,
        const VkDeviceSize& size, 
        const VkBufferUsageFlags& usage, 
        const VkMemoryPropertyFlags& properties, 
        VkBuffer* buffer,
        VkDeviceMemory* bufferMemory);
    void CopyBuffer(
        const VkDevice& device, 
        const VolkDeviceTable& table, 
        const VkPhysicalDevice& physicalDevice,
        const VkCommandPool& commandPool,
        const VkQueue& graphicsQueue,
        const VkDeviceSize& size,
        VkBuffer* srcBuffer, 
        VkBuffer* dstBuffer);
    VkCommandBuffer BeginSingleTimeCommands(
        const VkDevice& device, 
        const VolkDeviceTable& table,
        const VkCommandPool& commandPool);
    void EndSingleTimeCommands(
        const VkDevice& device, 
        const VolkDeviceTable& table,
        const VkQueue& graphicsQueue,
        const VkCommandPool& commandPool,
        const VkCommandBuffer& commandBuffer);
    bool CreateSharedBuffer(
        const VkDevice& device, 
        const VolkDeviceTable& table, 
        const VkPhysicalDevice& physicalDevice,
        const VkDeviceSize size, 
        const VkBufferUsageFlags usage, 
        const VkMemoryPropertyFlags properties, 
        QueueFamilyIndices& indices,
        VkBuffer* buffer, 
        VkDeviceMemory* bufferMemory);
    bool CreateUniformBuffer(
        const VkDevice& device, 
        const VolkDeviceTable& table, 
        const VkPhysicalDevice& physicalDevice,
        const VkDeviceSize& bufferSize,
        VkBuffer* buffer, 
        VkDeviceMemory* bufferMemory);
    void UploadBufferData(
        const VkDevice& device, 
        const VolkDeviceTable& table, 
        const VkDeviceMemory& bufferMemory, 
        VkDeviceSize deviceOffset, 
        const void* data, 
        const size_t dataSize);
    void DownloadBufferData(
        const VkDevice& device, 
        const VolkDeviceTable& table, 
        const VkDeviceMemory& bufferMemory, 
        VkDeviceSize deviceOffset, 
        void* outData, 
        const size_t dataSize);
}
