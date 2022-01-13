#include "Vulkan/VulkanLoader.h"
#include "Vulkan/VulkanUtils.h"
#include "Vulkan/VulkanConstant.h"
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
        instance = CreateVulkanInstance(validation_layers, instance_extensions);
        // Load volk instance
        volkLoadInstanceOnly(instance);
        // Print Version Info
        PrintVulkanVersion();
        // Setup Debug Messenger
#if defined(RK_DEBUG_INFO)
        debug_messenger = SetupDebugMessenger(instance);
        // report_callback = SetupDebugReportCallback(instance);
#endif
        // Create Sruface
        if (glfwCreateWindowSurface(instance, (GLFWwindow*)window, nullptr, &surface)) {
            RK_ERROR(Graphics, "Unable to Create Vulkan Surface");
        }
        RK_TRACE(Graphics, "Initialize Vulkan Loader");
    }

    void VulkanLoader::Finalize() {
        vkDestroySurfaceKHR(instance, surface, nullptr);
#if defined(RK_DEBUG_INFO)
        // vkDestroyDebugReportCallbackEXT(instance, report_callback, nullptr);
        vkDestroyDebugUtilsMessengerEXT(instance, debug_messenger, nullptr);
#endif
        vkDestroyInstance(instance, nullptr);
        RK_TRACE(Graphics, "Finalize Vulkan Loader");
    }
}
