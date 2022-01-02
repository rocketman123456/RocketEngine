#pragma once

#include <volk.h>

#include <vector>

struct QueueFamilyIndices;

namespace Rocket {
    // Debug Utils
    VkResult CreateDebugUtilsMessengerEXT(
        VkInstance instance, 
        const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, 
        const VkAllocationCallbacks* pAllocator, 
        VkDebugUtilsMessengerEXT* pDebugMessenger);
    void DestroyDebugUtilsMessengerEXT(
        VkInstance instance, 
        VkDebugUtilsMessengerEXT debugMessenger, 
        const VkAllocationCallbacks* pAllocator);
    // Create Instace
    VkInstance CreateVulkanInstance(const std::vector<const char*>& validationLayers);
    bool CheckValidationLayerSupport(const std::vector<const char*>& validationLayers);
    std::vector<const char*> GetRequiredExtensions(bool enable_debug);
    void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    // Print Version
    void PrintVulkanVersion();
    // Setup Debug Messenger
    VkDebugUtilsMessengerEXT SetupDebugMessenger(VkInstance instance, bool enableValidationLayers);
    VkDebugReportCallbackEXT SetupDebugReportCallback(VkInstance instance);
    // Pick Physical Device
    VkPhysicalDevice PickPhysicalDevice(VkInstance instance);
    bool IsDeviceSuitable(VkPhysicalDevice device);
    QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
    // Create Logical Device
    VkDevice CreateLogicalDevice(VkPhysicalDevice device, const std::vector<const char*>& validationLayers);
}
