#pragma once
#include <volk.h>

#include <vector>
#include <optional>

namespace Rocket {
    struct QueueFamilyIndices final {
        std::optional<uint32_t> multiplicity;
        std::optional<std::vector<uint32_t>> family_data;
        std::optional<uint32_t> present_family;
        std::optional<uint32_t> graphics_family;
        std::optional<uint32_t> compute_family;

        bool IsComplete();
        // Must Call After IsComplete
        uint32_t Multiplicity();
        const std::vector<uint32_t>& FamilyData();
    };

    struct SwapchainSupportDetails final {
        VkSurfaceCapabilitiesKHR capabilities = {};
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    struct VulkanInstance final {
        void* window;
        VkInstance instance;
        VkSurfaceKHR surface;
        VkDebugUtilsMessengerEXT messenger;
        VkDebugReportCallbackEXT report_callback;
    };

    struct VulkanRenderDevice final {
        void* window;
        uint32_t framebuffer_width;
        uint32_t framebuffer_height;

        VkDevice device;
        VkPhysicalDevice physical_device;
        VolkDeviceTable table;

        VkQueue present_queue;
        VkQueue graphics_queue;
        VkQueue compute_queue;

        // Were we initialized with compute capabilities
        // bool use_compute = false;
        // uint32_t graphics_family;
        // uint32_t present_family;
        // [may coincide with graphics_family]
        // uint32_t compute_family;
        QueueFamilyIndices family;

        // a list of all queues (for shared buffer allocation)
        std::vector<uint32_t> device_queue_indices;
        std::vector<VkQueue> device_queues;

        VkSwapchainKHR swapchain;
        VkFormat swapchain_image_format;
        std::vector<VkImage> swapchain_images;
        std::vector<VkImageView> swapchain_image_views;

        VkSemaphore semaphore;
        VkSemaphore render_semaphore;

        VkCommandPool command_pool;
        std::vector<VkCommandBuffer> command_buffers;
        VkCommandPool compute_command_pool;
        VkCommandBuffer compute_command_buffer;
    };

    // Features we need for our Vulkan context
    struct VulkanContextFeatures {
        bool support_screenshots = false;

        bool geometry_shader     = false;
        bool tessellation_shader = false;

        bool vertex_pipeline_stores_and_atomics = false;
        bool fragment_stores_and_atomics = false;
    };

    struct VulkanContextCreator {
        VulkanContextCreator() = default;

        VulkanContextCreator(
            VulkanInstance& vk, 
            VulkanRenderDevice& dev, 
            void* window, 
            int screenWidth, 
            int screenHeight, 
            const VulkanContextFeatures& ctxFeatures = VulkanContextFeatures());
        ~VulkanContextCreator();

        VulkanInstance& instance;
        VulkanRenderDevice& device;
    };

    struct VulkanShaderModule final {
        std::vector<unsigned int> SPIRV;
        VkShaderModule shader_module = nullptr;
    };

    struct VulkanBuffer {
        VkBuffer       buffer;
        VkDeviceSize   size;
        VkDeviceMemory memory;
        // Permanent mapping to CPU address space (see VulkanResources::addBuffer)
        void*          ptr;
    };

    struct VulkanImage final {
        VkImage image = nullptr;
        VkDeviceMemory image_memory = nullptr;
        VkImageView image_view = nullptr;
    };

    // Aggregate structure for passing around the texture data
    struct VulkanTexture final {
        uint32_t width;
        uint32_t height;
        uint32_t depth;
        VkFormat format;
        VulkanImage image;
        VkSampler sampler;
        // Offscreen buffers require VK_IMAGE_LAYOUT_GENERAL && static textures have VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
        VkImageLayout desired_layout;
    };

    enum eRenderPassBit : uint8_t {
        eRenderPassBit_First     = 0x01,
        eRenderPassBit_Last      = 0x02,
        eRenderPassBit_Offscreen = 0x04,
        eRenderPassBit_OffscreenInternal = 0x08,
    };

    struct VulkanRenderPassCreateInfo final {
        bool clear_color = false;
        bool clear_depth = false;
        uint8_t flags = 0;
    };

    struct VulkanRenderPass {
        VulkanRenderPass() = default;
        explicit VulkanRenderPass(
            VulkanRenderDevice& device, 
            bool useDepth = true, 
            const VulkanRenderPassCreateInfo& ci = VulkanRenderPassCreateInfo());

        VulkanRenderPassCreateInfo info;
        VkRenderPass handle = VK_NULL_HANDLE;
    };

    struct VulkanState {
        // 1. Descriptor set (layout + pool + sets) -> uses uniform buffers, textures, framebuffers
        VkDescriptorSetLayout descriptor_set_layout;
        VkDescriptorPool descriptor_pool;
        std::vector<VkDescriptorSet> descriptor_sets;

        // 2. 
        std::vector<VkFramebuffer> swapchain_framebuffers;

        // 3. Pipeline & render pass (using DescriptorSets & pipeline state options)
        VkRenderPass render_pass;
        VkPipelineLayout pipeline_layout;
        VkPipeline graphics_pipeline;

        // 4. Uniform buffer
        std::vector<VkBuffer> uniform_buffers;
        std::vector<VkDeviceMemory> uniform_buffers_memory;

        // 5. Storage Buffer with index and vertex data
        VkBuffer storage_buffer;
        VkDeviceMemory storage_buffer_memory;

        // 6. Depth buffer
        VulkanImage depth_texture;

        VkSampler texture_sampler;
        VulkanImage texture;
    };
}
