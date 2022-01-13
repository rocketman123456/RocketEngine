#pragma once
#include "Common/RenderLoader.h"
#include "Vulkan/VulkanVariable.h"

#include <volk.h>

namespace Rocket {
    class VulkanLoader : _implements_ RenderLoader {
    public:
        VulkanLoader() = default;
        virtual ~VulkanLoader() = default;
        void Initialize() final;
        void Finalize() final;
        void* GetWindow() final { return window; }
        void SetWindow(void* window) final { this->window = window; }
    public:
        void* window = nullptr;
        VkInstance instance;
        VkDebugUtilsMessengerEXT debug_messenger;
        VkDebugReportCallbackEXT report_callback;
        VkSurfaceKHR surface;
    };

    CLASS_PTR(VulkanLoader);
}
