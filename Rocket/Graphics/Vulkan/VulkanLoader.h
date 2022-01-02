#pragma once
#include "Common/RenderLoader.h"

#include <volk.h>

namespace Rocket {
    class VulkanLoader : _implements_ RenderLoader {
    public:
        VulkanLoader() = default;
        virtual ~VulkanLoader() = default;
        void Load() final;
        void Unload() final;
    public:
        VkInstance instance;
        VkSurfaceKHR surface;
        VkDebugUtilsMessengerEXT debug_messenger;
        VkDebugReportCallbackEXT report_callback;

        VkPhysicalDevice physical_device = VK_NULL_HANDLE;

        VkQueue graphics_queue;
        VkDevice device = VK_NULL_HANDLE;
        VolkDeviceTable device_table;
    };

    CLASS_PTR(VulkanLoader);
}
