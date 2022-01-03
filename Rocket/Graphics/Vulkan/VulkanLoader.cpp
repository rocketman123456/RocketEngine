#include "Vulkan/VulkanLoader.h"
#include "Vulkan/VulkanUtils.h"
#include "Vulkan/VulkanVariable.h"
#include "Log/Instrumentor.h"
#include "Log/Log.h"

#include <GLFW/glfw3.h>

#include <vector>
#include <exception>
#include <stdexcept>

namespace Rocket {
    void VulkanLoader::Initialize() {
        VkResult result = volkInitialize();
        if(result != VK_SUCCESS) {
            RK_ERROR(Graphics, "Unable to load Vulkan");
            throw std::runtime_error("Unable to load Vulkan");
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
        // Create Sruface
        if (glfwCreateWindowSurface(instance, (GLFWwindow*)window, nullptr, &surface)) {
            RK_ERROR(Graphics, "Unable to Create Vulkan Surface");
        }
        RK_TRACE(Graphics, "Initialize Vulkan Loader");
    }

    void VulkanLoader::Finalize() {
        vkDestroySurfaceKHR(instance, surface, nullptr);
        vkDestroyDebugReportCallbackEXT(instance, report_callback, nullptr);
        vkDestroyDebugUtilsMessengerEXT(instance, debug_messenger, nullptr);
        vkDestroyInstance(instance, nullptr);
        RK_TRACE(Graphics, "Finalize Vulkan Loader");
    }
}
