#pragma once
#include "Common/RenderDevice.h"
#include "Vulkan/VulkanLoader.h"
#include "Vulkan/VulkanUtils.h"

#include <volk.h>

namespace Rocket {
    class VulkanDevice : _implements_ RenderDevice {
    public:
        void SetLoader(RenderLoaderPtr loader) final;
        void Initialize() final;
        void Finalize() final;
    public:
        VulkanLoaderPtr loader;

        int32_t width = 0;
        int32_t height = 0;

        VkPhysicalDevice physical_device = VK_NULL_HANDLE;
        VkDevice device = VK_NULL_HANDLE;
        VolkDeviceTable device_table;

        QueueFamilyIndices indices;

        VkQueue graphics_queue;
        VkQueue present_queue;
    };

    CLASS_PTR(VulkanDevice);
}
