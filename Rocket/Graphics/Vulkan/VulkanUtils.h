#pragma once

#include <volk.h>

#include <vector>
#include <optional>

struct QueueFamilyIndices {
    std::optional<uint32_t> graphics_family;
    std::optional<uint32_t> present_family;
    std::optional<uint32_t> compute_family;
    bool IsComplete() {
        return 
            graphics_family.has_value() && 
            present_family.has_value() && 
            compute_family.has_value();
    }
};

struct SwapchainSupportDetails final {
    VkSurfaceCapabilitiesKHR capabilities = {};
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

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
        uint32_t width, 
        uint32_t height, 
        bool supportScreenshots);
}
