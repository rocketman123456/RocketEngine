#pragma once
#include "Vulkan/VulkanVariable.h"
#include "Vulkan/VulkanCheck.h"

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
        VkPhysicalDevice device, 
        VkSurfaceKHR surface);
    uint32_t FindPresentFamilies(
        VkPhysicalDevice device, 
        VkSurfaceKHR surface);
    uint32_t FindQueueFamilies(
        VkPhysicalDevice device, 
        VkQueueFlags desiredFlags);

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

    SwapchainSupportDetails QuerySwapchainSupport(
        const VkPhysicalDevice& device, 
        const VkSurfaceKHR& surface);
    VkSurfaceFormatKHR ChooseSwapSurfaceFormat(
        const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR ChooseSwapPresentMode(
        const std::vector<VkPresentModeKHR>& availablePresentModes);
    uint32_t ChooseSwapImageCount(const VkSurfaceCapabilitiesKHR& capabilities);
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
        VkSwapchainKHR* swapchain,
        uint32_t width, 
        uint32_t height, 
        bool supportScreenshots = false);
    
    VkResult CreateImageView(
        const VkDevice& device, 
        const VolkDeviceTable& table,
        const VkImage& image, 
        const VkFormat& format, 
        const VkImageAspectFlags& aspectFlags, 
        VkImageView* imageView, 
        VkImageViewType viewType = VK_IMAGE_VIEW_TYPE_2D, 
        uint32_t layerCount = 1, 
        uint32_t mipLevels = 1);

    std::size_t CreateSwapchainImages(
        const VkDevice& device,
        const VolkDeviceTable& table, 
        const VkSwapchainKHR& swapchain,
        const VkFormat& swapChainImageFormat,
        std::vector<VkImage>* swapchainImages,
        std::vector<VkImageView>* swapchainImageViews);

    VkResult CreateSemaphore(
        const VkDevice& device, 
        const VolkDeviceTable& table,
        VkSemaphore* outSemaphore);

    bool InitVulkanRenderDevice(
        VulkanInstance& vk, 
        VulkanRenderDevice& vkDev, 
        const std::vector<const char*>& deviceExtensions,
        const std::vector<const char*>& validationLayers,
        uint32_t width, 
        uint32_t height);

    void CleanupVulkanRenderDevice(VulkanRenderDevice& vkDev);
}
