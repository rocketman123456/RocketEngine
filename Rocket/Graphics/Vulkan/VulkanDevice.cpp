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

    void VulkanDevice::SetSize(uint32_t width, uint32_t height) {
        this->width = width;
        this->height = height;
    }

    void VulkanDevice::Initialize() {
        // Pick Physical Device
        physical_device = PickPhysicalDevice(
            loader->instance, loader->surface, device_extensions);
        // Create Logical Device
        indices.graphics_family = FindQueueFamilies(physical_device, VK_QUEUE_GRAPHICS_BIT);
        indices.compute_family = FindQueueFamilies(physical_device, VK_QUEUE_COMPUTE_BIT);
        indices.present_family = FindPresentFamilies(physical_device, loader->surface);
        // // Check Present Support
        if (!indices.IsComplete()) {
            RK_ERROR(Graphics, "Queue Families Not Supported");
            throw std::runtime_error("Queue Families Not Supported");
        }
        device = CreateLogicalDevice(
            physical_device, device_extensions, validation_layers, indices);
        // Load Volk Functions
        volkLoadDeviceTable(&device_table, device);
        // Create Graphics Queue
        device_table.vkGetDeviceQueue(
            device, indices.graphics_family.value(), 0, &graphics_queue);
        if (graphics_queue == nullptr) {
            RK_ERROR(Graphics, "Unable to Get Graphics Queue");
            throw std::runtime_error("Unable to Get Graphics Queue");
        }
        // Create Present Queue
        device_table.vkGetDeviceQueue(
            device, indices.present_family.value(), 0, &present_queue);
        if (present_queue == nullptr) {
            RK_ERROR(Graphics, "Unable to Get Present Queue");
            throw std::runtime_error("Unable to Get Present Queue");
        }
        // Create Compute Queue
        device_table.vkGetDeviceQueue(
            device, indices.compute_family.value(), 0, &compute_queue);
        if (compute_queue == nullptr) {
            RK_ERROR(Graphics, "Unable to Get Present Queue");
            throw std::runtime_error("Unable to Get Present Queue");
        }
        // Create Swap Chain
        swap_chain = CreateSwapChain(
            physical_device, device, device_table, 
            loader->surface, indices, 
            &swap_chain_image_format, 
            width, height, false);
        // Create Swap Chain Images
        const size_t imageCount = CreateSwapchainImages(
            device, device_table, swap_chain, swap_chain_image_format,
            swap_chain_images, swap_chain_images_views);
        command_buffers.resize(imageCount);
        // Create Semaphore
        CreateVulkanSemaphore(device, device_table, &semaphore);
        CreateVulkanSemaphore(device, device_table, &render_semaphore);
        // Create Command Pool
        CreateCommandPool(device, device_table, indices, &command_pool);
        CreateCommandBuffer(device, device_table, command_pool, imageCount, command_buffers.data());
        // Log
        RK_TRACE(Graphics, "Initialize Vulkan Device");
    }

    void VulkanDevice::Finalize() {
        device_table.vkDestroyCommandPool(device, command_pool, nullptr);
        device_table.vkDestroySemaphore(device, semaphore, nullptr);
        device_table.vkDestroySemaphore(device, render_semaphore, nullptr);
        for (auto imageView : swap_chain_images_views) {
            device_table.vkDestroyImageView(device, imageView, nullptr);
        }
        device_table.vkDestroySwapchainKHR(device, swap_chain, nullptr);
        device_table.vkDestroyDevice(device, nullptr);
        RK_TRACE(Graphics, "Finalize Vulkan Device");
    }
}
