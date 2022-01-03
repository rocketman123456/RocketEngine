#include "Vulkan/VulkanDevice.h"
#include "Vulkan/VulkanUtils.h"
#include "Vulkan/VulkanVariable.h"
#include "Utils/InstanceOf.h"
#include "Log/Instrumentor.h"
#include "Log/Log.h"

#include <exception>
#include <stdexcept>

namespace Rocket {
    void VulkanDevice::SetLoader(RenderLoaderPtr loader) {
        if(instanceof<VulkanLoader>(loader.get())) {
            this->loader = std::static_pointer_cast<VulkanLoader>(loader);
        } else {
            RK_ERROR(Graphics, "Unable to Convert Vulkan Loader");
            throw std::runtime_error("Unable to Convert Vulkan Loader");
        }
    }

    void VulkanDevice::Initialize() {
        // Pick Physical Device
        physical_device = PickPhysicalDevice(this->loader->instance);
        // Create Logical Device
        indices = FindQueueFamilies(physical_device);
        device = CreateLogicalDevice(physical_device, validation_layers, indices);
        // Load Volk Functions
        volkLoadDeviceTable(&device_table, device);
        // Create Graphics Queue
        vkGetDeviceQueue(device, indices.graphics_family.value(), 0, &graphics_queue);
        if (graphics_queue == nullptr) {
            RK_ERROR(Graphics, "Unable to Get Graphics Queue");
            throw std::runtime_error("Unable to Get Graphics Queue");
        }
        // Check Present Support
        VkBool32 present_supported = 0;
        vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, indices.graphics_family.value(), this->loader->surface, &present_supported);
        if (!present_supported) {
            RK_ERROR(Graphics, "Present Queue Not Supported");
            throw std::runtime_error("Present Queue Not Supported");
        }
    }

    void VulkanDevice::Finalize() {
        vkDestroyDevice(device, nullptr);
    }
}
