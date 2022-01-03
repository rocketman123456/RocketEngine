#pragma once

#include <volk.h>

#include <vector>
#include <optional>

struct QueueFamilyIndices {
    std::optional<uint32_t> graphics_family;
    std::optional<uint32_t> compute_family;
    bool IsComplete() {
        return graphics_family.has_value();
    }
};

struct SwapchainSupportDetails final {
    VkSurfaceCapabilitiesKHR capabilities = {};
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

namespace Rocket {
    // Create Instace
    VkInstance CreateVulkanInstance(const std::vector<const char*>& validationLayers);
    bool CheckValidationLayerSupport(const std::vector<const char*>& validationLayers);
    std::vector<const char*> GetRequiredExtensions(bool enable_debug);
    void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    void PopulateDebugReportCreateInfo(VkDebugReportCallbackCreateInfoEXT& createInfo);
    // Print Version
    void PrintVulkanVersion();
    // Setup Debug Messenger
    VkDebugUtilsMessengerEXT SetupDebugMessenger(
        const VkInstance& instance, bool enableValidationLayers);
    VkDebugReportCallbackEXT SetupDebugReportCallback(const VkInstance& instance);
    // Pick Physical Device
    VkPhysicalDevice PickPhysicalDevice(const VkInstance& instance);
    bool IsDeviceSuitable(const VkPhysicalDevice& device);
    // Create Logical Device
    VkDevice CreateLogicalDevice(
        const VkPhysicalDevice& device, 
        const std::vector<const char*>& validationLayers, 
        const QueueFamilyIndices& indices);
    QueueFamilyIndices FindQueueFamilies(const VkPhysicalDevice& device);
    // Create Swap Chain
    VkSwapchainKHR CreateSwapChain(
        const VkPhysicalDevice& physicalDevice, 
        const VkDevice& device,
        const VolkDeviceTable& table,
        const VkSurfaceKHR& surface, 
        const QueueFamilyIndices& indices, 
        uint32_t width, 
        uint32_t height, 
        bool supportScreenshots);
    SwapchainSupportDetails QuerySwapchainSupport(
        VkPhysicalDevice device, VkSurfaceKHR surface);
    VkSurfaceFormatKHR ChooseSwapSurfaceFormat(
        const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR ChooseSwapPresentMode(
        const std::vector<VkPresentModeKHR>& availablePresentModes);
    uint32_t ChooseSwapImageCount(const VkSurfaceCapabilitiesKHR& capabilities);
    VkExtent2D ChooseSwapExtent(
        const VkSurfaceCapabilitiesKHR& capabilities, 
        uint32_t width, uint32_t height);
}
