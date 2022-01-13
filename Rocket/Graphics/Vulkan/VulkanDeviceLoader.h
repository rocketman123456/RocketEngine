#pragma once
#include "Common/RenderDeviceLoader.h"
#include "Vulkan/VulkanLoader.h"
#include "Vulkan/VulkanUtils.h"

#include <volk.h>

namespace Rocket {
    class VulkanDeviceLoader : _implements_ RenderDeviceLoader {
    public:
        void SetLoader(RenderLoaderPtr loader) final;
        void SetSize(uint32_t width, uint32_t height) final;
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
        VkQueue compute_queue;

        std::vector<uint32_t> device_queue_indices;
        std::vector<VkQueue> device_queues;

        VkSwapchainKHR swap_chain;
        VkFormat swap_chain_image_format;
        VkExtent2D swap_chain_extent;
        std::vector<VkImage> swap_chain_images;
        std::vector<VkImageView> swap_chain_images_views;
        std::vector<VkFramebuffer> swap_chain_framebuffers;

        VkSemaphore semaphore;
        VkSemaphore render_semaphore;

        std::vector<VkSemaphore> imageAvailableSemaphores;
        std::vector<VkSemaphore> renderFinishedSemaphores;
        std::vector<VkFence> inFlightFences;
        std::vector<VkFence> imagesInFlight;
        size_t currentFrame = 0;

        VkCommandPool command_pool;
        std::vector<VkCommandBuffer> command_buffers;

        VkCommandPool compute_command_pool;
        VkCommandBuffer compute_command_buffer;
    };

    CLASS_PTR(VulkanDeviceLoader);
}
