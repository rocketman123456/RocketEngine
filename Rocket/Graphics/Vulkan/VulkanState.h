#pragma once
#include "Common/RenderState.h"

#include <volk.h>
#include <vector>

namespace Rocket {
    class VulkanState : _implements_ RenderState {
    public:
        void Initialize() final;
        void Finalize() final;
    public:
        // 1. Descriptor set (layout + pool + sets) -> uses uniform buffers, textures, framebuffers
        VkDescriptorSetLayout descriptorSetLayout;
        VkDescriptorPool descriptorPool;
        std::vector<VkDescriptorSet> descriptorSets;

        // 2. Swapchain
        std::vector<VkFramebuffer> swapchainFramebuffers;

        // 3. Pipeline & render pass (using DescriptorSets & pipeline state options)
        VkRenderPass renderPass;
        VkPipelineLayout pipelineLayout;
        VkPipeline graphicsPipeline;

        // 4. Uniform buffer
        std::vector<VkBuffer> uniformBuffers;
        std::vector<VkDeviceMemory> uniformBuffersMemory;

        // 5. Storage Buffer with index and vertex data
        VkBuffer storageBuffer;
        VkDeviceMemory storageBufferMemory;

        // 6. Depth buffer
        // VulkanImage depthTexture;

        // 7. Texture Related
        VkSampler textureSampler;
        // VulkanImage texture;
    };
}