#pragma once
#include "Vulkan/VulkanVariable.h"
#include "Vulkan/VulkanCheck.h"

#include <vector>
#include <string>

namespace Rocket {
    bool CheckDeviceExtensionSupport(
        const VkPhysicalDevice& device, 
        const std::vector<const char*>& deviceExtensions);
    bool IsDeviceSuitable(
        const VkPhysicalDevice& device, 
        const VkSurfaceKHR& surface, 
        const std::vector<const char*>& deviceExtensions);
    VkResult PickPhysicalDevice(
        const VkInstance& instance, 
        const VkSurfaceKHR& surface, 
        const std::vector<const char*>& deviceExtensions,
        VkPhysicalDevice* physical_device);

    QueueFamilyIndices FindQueueFamilyIndices(
        const VkPhysicalDevice& device, 
        const VkSurfaceKHR& surface);
    uint32_t FindPresentFamilies(
        const VkPhysicalDevice& device, 
        const VkSurfaceKHR& surface);
    uint32_t FindQueueFamilies(
        const VkPhysicalDevice& device, 
        const VkQueueFlags& desiredFlags);

    VkResult CreateLogicalDevice(
        VkPhysicalDevice physicalDevice, 
        VkPhysicalDeviceFeatures deviceFeatures, 
        const std::vector<const char*>& validationLayers,
        const std::vector<const char*>& deviceExtensions,
        uint32_t graphicsFamily, 
        VkDevice* device);
    VkResult CreateLogicalDeviceWithCompute(
        VkPhysicalDevice physicalDevice, 
        VkPhysicalDeviceFeatures deviceFeatures, 
        const std::vector<const char*>& validationLayers,
        const std::vector<const char*>& deviceExtensions,
        uint32_t graphicsFamily, 
        uint32_t computeFamily, 
        VkDevice* device);
    VkResult CreateLogicalDevice(
        const VkPhysicalDevice& physicalDevice, 
        const std::vector<const char*>& validationLayers,
        const std::vector<const char*>& deviceExtensions, 
        const QueueFamilyIndices& indices,
        VkDevice* device);
    
    VkResult GetVulkanQueue(
        const VkDevice& device,
        const VolkDeviceTable& table,
        uint32_t index,
        VkQueue* queue);

    SwapchainSupportDetails QuerySwapchainSupport(
        const VkPhysicalDevice& device, 
        const VkSurfaceKHR& surface);
    VkSurfaceFormatKHR ChooseSwapSurfaceFormat(
        const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR ChooseSwapPresentMode(
        const std::vector<VkPresentModeKHR>& availablePresentModes);
    uint32_t ChooseSwapImageCount(
        const VkSurfaceCapabilitiesKHR& capabilities);
    VkExtent2D ChooseSwapExtent(
        const VkSurfaceCapabilitiesKHR& capabilities, 
        uint32_t width, 
        uint32_t height);

    VkResult CreateSwapchain(
        const VkPhysicalDevice& physicalDevice, 
        const VkDevice& device,
        const VolkDeviceTable& table,
        const VkSurfaceKHR& surface, 
        const QueueFamilyIndices& indices, 
        VkFormat* swapChainImageFormat,
        VkExtent2D* swapchainExtent,
        VkSwapchainKHR* swapchain,
        uint32_t width, 
        uint32_t height, 
        bool supportScreenshots = false);

    std::size_t CreateSwapchainImages(
        const VkDevice& device,
        const VolkDeviceTable& table, 
        const VkSwapchainKHR& swapchain,
        const VkFormat& swapChainImageFormat,
        std::vector<VkImage>* swapchainImages,
        std::vector<VkImageView>* swapchainImageViews);

    VkResult InitVulkanRenderDevice(
        VulkanInstance& vk, 
        VulkanRenderDevice& vkDev, 
        const std::vector<const char*>& deviceExtensions,
        const std::vector<const char*>& validationLayers,
        uint32_t framebuffer_width, 
        uint32_t framebuffer_height);

    void CleanupVulkanRenderDevice(
        VulkanRenderDevice& vkDev);

//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------

    uint32_t FindMemoryType(
        const VkPhysicalDevice& device, 
        uint32_t typeFilter, 
        VkMemoryPropertyFlags properties);

    VkCommandBuffer BeginSingleTimeCommands(VulkanRenderDevice& vkDev);
    void EndSingleTimeCommands(VulkanRenderDevice& vkDev, VkCommandBuffer commandBuffer);

//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------

    VkResult CreateDepthResources(
        VulkanRenderDevice& vkDev, 
        uint32_t width, 
        uint32_t height, 
        VulkanImage& depth);

    VkResult CreateDescriptorPool(
        VulkanRenderDevice& vkDev, 
        uint32_t uniformBufferCount, 
        uint32_t storageBufferCount, 
        uint32_t samplerCount, 
        VkDescriptorPool* descriptorPool);
    
    VkDescriptorSetLayoutBinding DescriptorSetLayoutBinding(
        uint32_t binding, 
        VkDescriptorType descriptorType, 
        VkShaderStageFlags stageFlags, 
        uint32_t descriptorCount = 1);

    VkResult CreateDescriptorSet(
        VulkanRenderDevice& vkDev, 
        VulkanState* vkState,
        uint32_t uniformBufferSize,
        uint32_t vertexBufferSize,
        uint32_t indexBufferSize);
    
    VkResult CreateColorAndDepthRenderPass(
        VulkanRenderDevice& vkDev, 
        const bool useDepth, 
        VkRenderPass* renderPass, 
        const VulkanRenderPassCreateInfo& ci, 
        const VkFormat& colorFormat = VK_FORMAT_B8G8R8A8_UNORM);
    
    VkResult CreateColorOnlyRenderPass(
        VulkanRenderDevice& vkDev, 
        VkRenderPass* renderPass, 
        const VulkanRenderPassCreateInfo& ci, 
        const VkFormat& colorFormat = VK_FORMAT_B8G8R8A8_UNORM);

    VkResult CreateDepthOnlyRenderPass(
        VulkanRenderDevice& vkDev, 
        VkRenderPass* renderPass, 
        const VulkanRenderPassCreateInfo& ci);

    VkResult CreatePipelineLayout(
        const VkDevice& device, 
        const VolkDeviceTable& table, 
        VkDescriptorSetLayout dsLayout, 
        VkPipelineLayout* pipelineLayout);

    VkPipelineShaderStageCreateInfo ShaderStageInfo(
        VkShaderStageFlagBits shaderStage, 
        VulkanShaderModule& module, 
        const char* entryPoint);

    VkResult CreateGraphicsPipeline(
        VulkanRenderDevice& vkDev,
        VkRenderPass renderPass, 
        VkPipelineLayout pipelineLayout,
        const std::vector<std::string>& shaderFiles,
        VkPipeline* pipeline,
        /* defaults to triangles*/
        bool useDepth = true,
        bool useBlending = true,
        bool dynamicScissorState = false,
        VkPrimitiveTopology topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
        int32_t customWidth  = -1,
        int32_t customHeight = -1,
        uint32_t numPatchControlPoints = 0);

    VkResult CreateColorAndDepthFramebuffers(
        VulkanRenderDevice& vkDev, 
        VkRenderPass renderPass, 
        VkImageView depthImageView, 
        std::vector<VkFramebuffer>& swapchainFramebuffers);

    VkResult CreateColorAndDepthFramebuffer(
        VulkanRenderDevice& vkDev,
        uint32_t width, 
        uint32_t height,
        VkRenderPass renderPass, 
        VkImageView colorImageView, 
        VkImageView depthImageView,
        VkFramebuffer* framebuffer);

    VkResult CreateDepthOnlyFramebuffer(
        VulkanRenderDevice& vkDev,
        uint32_t width, 
        uint32_t height,
        VkRenderPass renderPass, 
        VkImageView depthImageView,
        VkFramebuffer* framebuffer);

}
