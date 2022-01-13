#pragma once
#include "Vulkan/VulkanVariable.h"

#include <glslang/Include/glslang_c_interface.h>

#include <functional>

#define VK_CHECK(value) CHECK(value == VK_SUCCESS, __FILE__, __LINE__);
#define VK_CHECK_RET(value) if ( value != VK_SUCCESS ) { CHECK(false, __FILE__, __LINE__); return value; }
#define BL_CHECK(value) CHECK(value, __FILE__, __LINE__);

namespace Rocket {
    void CHECK(bool check, const char* fileName, int lineNumber);

    void PrintVulkanVersion();

    void SetupDebugMessenger(
        const VkInstance& instance, VkDebugUtilsMessengerEXT* messenger);
    void SetupDebugReportCallback(
        const VkInstance& instance, VkDebugReportCallbackEXT* reportCallback);

    VkShaderStageFlagBits glslangShaderStageToVulkan(glslang_stage_t sh);
    glslang_stage_t glslangShaderStageFromFileName(const char* fileName);

    size_t CompileShader(
        glslang_stage_t stage, 
        const char* shaderSource, 
        VulkanShaderModule* shaderModule);

    size_t CompileShaderFile(
        const char* root, 
        const char* file, 
        VulkanShaderModule* shaderModule);

    VkResult CreateShaderModule(
        VkDevice device, 
        VulkanShaderModule* shader, 
        const char* root, 
        const char* fileName);

    void CreateInstance(VkInstance* instance);

    VkResult CreateDevice(
        VkPhysicalDevice physicalDevice, 
        VkPhysicalDeviceFeatures deviceFeatures, 
        uint32_t graphicsFamily, 
        VkDevice* device);

    VkResult CreateSwapchain(
        VkDevice device, 
        VkPhysicalDevice physicalDevice, 
        VkSurfaceKHR surface, 
        uint32_t graphicsFamily, 
        uint32_t width, 
        uint32_t height, 
        VkSwapchainKHR* swapchain, 
        bool supportScreenshots = false);

    size_t CreateSwapchainImages(
        VkDevice device, 
        VkSwapchainKHR swapchain, 
        std::vector<VkImage>& swapchainImages, 
        std::vector<VkImageView>& swapchainImageViews);

    VkResult CreateSemaphore(
        VkDevice device, 
        VkSemaphore* outSemaphore);

    bool CreateTextureSampler(
        VkDevice device, 
        VkSampler* sampler, 
        VkFilter minFilter = VK_FILTER_LINEAR, 
        VkFilter maxFilter = VK_FILTER_LINEAR, 
        VkSamplerAddressMode addressMode = VK_SAMPLER_ADDRESS_MODE_REPEAT);

    bool CreateDescriptorPool(
        VulkanRenderDevice& vkDev, 
        uint32_t uniformBufferCount, 
        uint32_t storageBufferCount, 
        uint32_t samplerCount, 
        VkDescriptorPool* descriptorPool);

    bool IsDeviceSuitable(VkPhysicalDevice device);

    SwapchainSupportDetails QuerySwapchainSupport(
        VkPhysicalDevice device, 
        VkSurfaceKHR surface);

    VkSurfaceFormatKHR ChooseSwapSurfaceFormat(
        const std::vector<VkSurfaceFormatKHR>& availableFormats);

    VkPresentModeKHR ChooseSwapPresentMode(
        const std::vector<VkPresentModeKHR>& availablePresentModes);

    uint32_t ChooseSwapImageCount(
        const VkSurfaceCapabilitiesKHR& capabilities);

    VkResult FindSuitablePhysicalDevice(
        VkInstance instance, 
        std::function<bool(VkPhysicalDevice)> selector, 
        VkPhysicalDevice* physicalDevice);

    uint32_t FindPresentFamilies(
        VkPhysicalDevice device, 
        VkSurfaceKHR surface);

    uint32_t FindQueueFamilies(
        VkPhysicalDevice device, 
        VkQueueFlags desiredFlags);

    VkFormat FindSupportedFormat(
        VkPhysicalDevice device, 
        const std::vector<VkFormat>& candidates, 
        VkImageTiling tiling, 
        VkFormatFeatureFlags features);

    uint32_t FindMemoryType(
        VkPhysicalDevice device, 
        uint32_t typeFilter, 
        VkMemoryPropertyFlags properties);

    VkFormat FindDepthFormat(VkPhysicalDevice device);

    bool HasStencilComponent(VkFormat format);

    VkPipelineShaderStageCreateInfo ShaderStageInfo(
        VkShaderStageFlagBits shaderStage, 
        const VulkanShaderModule& module, 
        const char* entryPoint);

    VkDescriptorSetLayoutBinding DescriptorSetLayoutBinding(
        uint32_t binding, 
        VkDescriptorType descriptorType, 
        VkShaderStageFlags stageFlags, 
        uint32_t descriptorCount = 1);
    
    VkWriteDescriptorSet BufferWriteDescriptorSet(
        VkDescriptorSet ds, 
        const VkDescriptorBufferInfo* bi, 
        uint32_t bindIdx, 
        VkDescriptorType dType);
    
    VkWriteDescriptorSet ImageWriteDescriptorSet(
        VkDescriptorSet ds, 
        const VkDescriptorImageInfo* ii, 
        uint32_t bindIdx);

    uint32_t GetVulkanBufferAlignment(VulkanRenderDevice& vkDev);

    bool IsDepthFormat(VkFormat fmt);

    bool SetVkObjectName(
        VulkanRenderDevice& vkDev, 
        void* object, 
        VkObjectType objType, 
        const char* name);

    bool SetVkImageName(
        VulkanRenderDevice& vkDev, 
        void* object, 
        const char* name);

    void UpdateTextureInDescriptorSetArray(
        VulkanRenderDevice& vkDev, 
        VkDescriptorSet ds, 
        VulkanTexture t, 
        uint32_t textureIndex, 
        uint32_t bindingIdx);

    bool CreateGraphicsPipeline(
        VulkanRenderDevice& vkDev,
        VkRenderPass renderPass, VkPipelineLayout pipelineLayout,
        const std::vector<const char*>& shaderFiles,
        VkPipeline* pipeline,
        /* defaults to triangles*/
        VkPrimitiveTopology topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
        bool useDepth = true,
        bool useBlending = true,
        bool dynamicScissorState = false,
        int32_t customWidth  = -1,
        int32_t customHeight = -1,
        uint32_t numPatchControlPoints = 0);

    VkResult CreateComputePipeline(
        VkDevice device, 
        VkShaderModule computeShader, 
        VkPipelineLayout pipelineLayout, 
        VkPipeline* pipeline);

    bool CreateSharedBuffer(
        VulkanRenderDevice& vkDev, 
        VkDeviceSize size, 
        VkBufferUsageFlags usage, 
        VkMemoryPropertyFlags properties, 
        VkBuffer* buffer, 
        VkDeviceMemory* bufferMemory);

    bool createBuffer(
        VkDevice device, 
        VkPhysicalDevice physicalDevice, 
        VkDeviceSize size, 
        VkBufferUsageFlags usage, 
        VkMemoryPropertyFlags properties, 
        VkBuffer* buffer, 
        VkDeviceMemory* bufferMemory);
    bool createImage(VkDevice device, VkPhysicalDevice physicalDevice, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory, VkImageCreateFlags flags = 0, uint32_t mipLevels = 1);

    bool createVolume(VkDevice device, VkPhysicalDevice physicalDevice, uint32_t width, uint32_t height, uint32_t depth,
        VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory, VkImageCreateFlags flags);

    bool createOffscreenImage(VulkanRenderDevice& vkDev,
            VkImage& textureImage, VkDeviceMemory& textureImageMemory,
            uint32_t texWidth, uint32_t texHeight,
            VkFormat texFormat,
            uint32_t layerCount, VkImageCreateFlags flags);

    bool createOffscreenImageFromData(VulkanRenderDevice& vkDev,
            VkImage& textureImage, VkDeviceMemory& textureImageMemory,
            void* imageData, uint32_t texWidth, uint32_t texHeight,
            VkFormat texFormat,
            uint32_t layerCount, VkImageCreateFlags flags);

    bool createDepthSampler(VkDevice device, VkSampler* sampler);

    bool createUniformBuffer(VulkanRenderDevice& vkDev, VkBuffer& buffer, VkDeviceMemory& bufferMemory, VkDeviceSize bufferSize);

    /** Copy [data] to GPU device buffer */
    void uploadBufferData(VulkanRenderDevice& vkDev, const VkDeviceMemory& bufferMemory, VkDeviceSize deviceOffset, const void* data, const size_t dataSize);

    /** Copy GPU device buffer data to [outData] */
    void downloadBufferData(VulkanRenderDevice& vkDev, const VkDeviceMemory& bufferMemory, VkDeviceSize deviceOffset, void* outData, size_t dataSize);

    bool createImageView(VkDevice device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, VkImageView* imageView, VkImageViewType viewType = VK_IMAGE_VIEW_TYPE_2D, uint32_t layerCount = 1, uint32_t mipLevels = 1);

    bool CreateColorOnlyRenderPass(
        VulkanRenderDevice& device, 
        VkRenderPass* renderPass, 
        const VulkanRenderPassCreateInfo& ci, 
        VkFormat colorFormat = VK_FORMAT_B8G8R8A8_UNORM);
    bool CreateColorAndDepthRenderPass(
        VulkanRenderDevice& device, 
        bool useDepth, 
        VkRenderPass* renderPass, 
        const VulkanRenderPassCreateInfo& ci, 
        VkFormat colorFormat = VK_FORMAT_B8G8R8A8_UNORM);
    bool CreateDepthOnlyRenderPass(
        VulkanRenderDevice& vkDev, 
        VkRenderPass* renderPass, 
        const VulkanRenderPassCreateInfo& ci);

    VkCommandBuffer beginSingleTimeCommands(VulkanRenderDevice& vkDev);
    void endSingleTimeCommands(VulkanRenderDevice& vkDev, VkCommandBuffer commandBuffer);
    void copyBuffer(VulkanRenderDevice& vkDev, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    void transitionImageLayout(VulkanRenderDevice& vkDev, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t layerCount = 1, uint32_t mipLevels = 1);
    void transitionImageLayoutCmd(VkCommandBuffer commandBuffer, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t layerCount = 1, uint32_t mipLevels = 1);

    bool initVulkanRenderDevice(VulkanInstance& vk, VulkanRenderDevice& vkDev, uint32_t width, uint32_t height, std::function<bool(VkPhysicalDevice)> selector, VkPhysicalDeviceFeatures deviceFeatures);
    bool initVulkanRenderDevice2(VulkanInstance& vk, VulkanRenderDevice& vkDev, uint32_t width, uint32_t height, std::function<bool(VkPhysicalDevice)> selector, VkPhysicalDeviceFeatures2 deviceFeatures2);
    bool initVulkanRenderDevice3(VulkanInstance& vk, VulkanRenderDevice& vkDev, uint32_t width, uint32_t height, const VulkanContextFeatures& ctxFeatures = VulkanContextFeatures());
    void destroyVulkanRenderDevice(VulkanRenderDevice& vkDev);
    void destroyVulkanInstance(VulkanInstance& vk);

    bool initVulkanRenderDeviceWithCompute(VulkanInstance& vk, VulkanRenderDevice& vkDev, uint32_t width, uint32_t height, VkPhysicalDeviceFeatures deviceFeatures);

    bool initVulkanRenderDevice2WithCompute(VulkanInstance& vk, VulkanRenderDevice& vkDev, uint32_t width, uint32_t height, std::function<bool(VkPhysicalDevice)> selector, VkPhysicalDeviceFeatures2 deviceFeatures2, bool supportScreenshots = false);

    bool createColorAndDepthFramebuffers(VulkanRenderDevice& vkDev, VkRenderPass renderPass, VkImageView depthImageView, std::vector<VkFramebuffer>& swapchainFramebuffers);
    bool createColorAndDepthFramebuffer(VulkanRenderDevice& vkDev,
        uint32_t width, uint32_t height,
        VkRenderPass renderPass, VkImageView colorImageView, VkImageView depthImageView,
        VkFramebuffer* framebuffer);
    bool createDepthOnlyFramebuffer(VulkanRenderDevice& vkDev,
        uint32_t width, uint32_t height,
        VkRenderPass renderPass, VkImageView depthImageView,
        VkFramebuffer* framebuffer);

    void copyBufferToImage(VulkanRenderDevice& vkDev, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t layerCount = 1);
    void copyImageToBuffer(VulkanRenderDevice& vkDev, VkImage image, VkBuffer buffer, uint32_t width, uint32_t height, uint32_t layerCount = 1);

    void copyMIPBufferToImage(VulkanRenderDevice& vkDev, VkBuffer buffer, VkImage image, uint32_t mipLevels, uint32_t width, uint32_t height, uint32_t bytesPP, uint32_t layerCount = 1);

    void destroyVulkanImage(VkDevice device, VulkanImage& image);
    void destroyVulkanTexture(VkDevice device, VulkanTexture& texture);

    uint32_t bytesPerTexFormat(VkFormat fmt);

    /* VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL for real update of an existing texture */
    bool updateTextureImage(VulkanRenderDevice& vkDev, VkImage& textureImage, VkDeviceMemory& textureImageMemory, uint32_t texWidth, uint32_t texHeight, VkFormat texFormat, uint32_t layerCount, const void* imageData, VkImageLayout sourceImageLayout = VK_IMAGE_LAYOUT_UNDEFINED);

    bool updateTextureVolume(VulkanRenderDevice& vkDev, VkImage& textureVolume, VkDeviceMemory& textureVolumeMemory, uint32_t texWidth, uint32_t texHeight, uint32_t texDepth, VkFormat texFormat, const void* volumeData, VkImageLayout sourceImageLayout = VK_IMAGE_LAYOUT_UNDEFINED);

    bool downloadImageData(VulkanRenderDevice& vkDev, VkImage& textureImage, uint32_t texWidth, uint32_t texHeight, VkFormat texFormat, uint32_t layerCount, void* imageData, VkImageLayout sourceImageLayout);

    bool createDepthResources(VulkanRenderDevice& vkDev, uint32_t width, uint32_t height, VulkanImage& depth);

    bool createPipelineLayout(VkDevice device, VkDescriptorSetLayout dsLayout, VkPipelineLayout* pipelineLayout);

    bool createPipelineLayoutWithConstants(VkDevice device, VkDescriptorSetLayout dsLayout, VkPipelineLayout* pipelineLayout, uint32_t vtxConstSize, uint32_t fragConstSize);

    bool createTextureImageFromData(VulkanRenderDevice& vkDev,
            VkImage& textureImage, VkDeviceMemory& textureImageMemory,
            void* imageData, uint32_t texWidth, uint32_t texHeight,
            VkFormat texFormat,
            uint32_t layerCount = 1, VkImageCreateFlags flags = 0);

    bool createMIPTextureImageFromData(VulkanRenderDevice& vkDev,
            VkImage& textureImage, VkDeviceMemory& textureImageMemory,
            void* mipData, uint32_t mipLevels, uint32_t texWidth, uint32_t texHeight,
            VkFormat texFormat,
            uint32_t layerCount = 1, VkImageCreateFlags flags = 0);

    bool createTextureVolumeFromData(VulkanRenderDevice& vkDev,
            VkImage& textureVolume, VkDeviceMemory& textureVolumeMemory,
            void* volumeData, uint32_t texWidth, uint32_t texHeight, uint32_t texDepth,
            VkFormat texFormat,
            VkImageCreateFlags flags = 0);

    bool createTextureImage(VulkanRenderDevice& vkDev, const char* filename, VkImage& textureImage, VkDeviceMemory& textureImageMemory, uint32_t* outTexWidth = nullptr, uint32_t* outTexHeight = nullptr);

    bool createMIPTextureImage(VulkanRenderDevice& vkDev, const char* filename, uint32_t mipLevels, VkImage& textureImage, VkDeviceMemory& textureImageMemory, uint32_t* width = nullptr, uint32_t* height = nullptr);

    bool createCubeTextureImage(VulkanRenderDevice& vkDev, const char* filename, VkImage& textureImage, VkDeviceMemory& textureImageMemory, uint32_t* width = nullptr, uint32_t* height = nullptr);

    bool createMIPCubeTextureImage(VulkanRenderDevice& vkDev, const char* filename, uint32_t mipLevels, VkImage& textureImage, VkDeviceMemory& textureImageMemory, uint32_t* width = nullptr, uint32_t* height = nullptr);

    size_t allocateVertexBuffer(VulkanRenderDevice& vkDev, VkBuffer* storageBuffer, VkDeviceMemory* storageBufferMemory, size_t vertexDataSize, const void* vertexData, size_t indexDataSize, const void* indexData);

    bool createTexturedVertexBuffer(VulkanRenderDevice& vkDev, const char* filename, VkBuffer* storageBuffer, VkDeviceMemory* storageBufferMemory, size_t* vertexBufferSize, size_t* indexBufferSize);

    bool createPBRVertexBuffer(VulkanRenderDevice& vkDev, const char* filename, VkBuffer* storageBuffer, VkDeviceMemory* storageBufferMemory, size_t* vertexBufferSize, size_t* indexBufferSize);

    bool executeComputeShader(VulkanRenderDevice& vkDev,
        VkPipeline computePipeline, VkPipelineLayout pl, VkDescriptorSet ds,
        uint32_t xsize, uint32_t ysize, uint32_t zsize);

    bool createComputeDescriptorSetLayout(VkDevice device, VkDescriptorSetLayout* descriptorSetLayout);

    void insertComputedBufferBarrier(VulkanRenderDevice& vkDev, VkCommandBuffer commandBuffer, VkBuffer buffer);
    void insertComputedImageBarrier(VkCommandBuffer commandBuffer, VkImage image);

    VkSampleCountFlagBits getMaxUsableSampleCount(VkPhysicalDevice physDevice);
}
