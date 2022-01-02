#include "Vulkan/VulkanLoader.h"
#include "Vulkan/VulkanUtils.h"
#include "Log/Instrumentor.h"
#include "Log/Log.h"

#include <vector>

static const std::vector<const char*> validation_layers = {
#ifdef RK_DEBUG_INFO
    "VK_LAYER_KHRONOS_validation"
#endif
};

namespace Rocket {
    void VulkanLoader::Load() {
        VkResult result = volkInitialize();
        if(result != VK_SUCCESS) {
            RK_ERROR(Graphics, "Unable to load Vulkan");
        }
        // Get Vulkan Instance
        instance = CreateVulkanInstance(validation_layers);
        // Load volk instance
        volkLoadInstance(instance);
        // Print Version Info
        PrintVulkanVersion();
        // Setup Debug Messenger
        bool enable_validation = validation_layers.size() > 0;
        debug_messenger = SetupDebugMessenger(instance, enable_validation);
        report_callback = SetupDebugReportCallback(instance);
        // Pick Physical Device
        physical_device = PickPhysicalDevice(instance);
        // Create Logical Device
        device = CreateLogicalDevice(physical_device, validation_layers);
        // Load Volk Functions
        // TODO : make device_table able to support multi-GPU
        volkLoadDeviceTable(&device_table, device);
    }

    void VulkanLoader::Unload() {
        vkDestroyDevice(device, nullptr);
        DestroyDebugUtilsMessengerEXT(instance, debug_messenger, nullptr);
        vkDestroyInstance(instance, nullptr);
        RK_TRACE(Graphics, "Unload Vulkan");
    }
}
