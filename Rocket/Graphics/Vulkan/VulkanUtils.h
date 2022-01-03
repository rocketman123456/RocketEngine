#pragma once

#include <volk.h>

#include <vector>
#include <optional>

struct QueueFamilyIndices {
    std::optional<uint32_t> graphics_family;
    bool IsComplete() {
        return graphics_family.has_value();
    }
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
    VkDebugUtilsMessengerEXT SetupDebugMessenger(VkInstance instance, bool enableValidationLayers);
    VkDebugReportCallbackEXT SetupDebugReportCallback(VkInstance instance);
    // Pick Physical Device
    VkPhysicalDevice PickPhysicalDevice(VkInstance instance);
    bool IsDeviceSuitable(VkPhysicalDevice device);
    // Create Logical Device
    VkDevice CreateLogicalDevice(VkPhysicalDevice device, const std::vector<const char*>& validationLayers, const QueueFamilyIndices& indices);
    QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
}
