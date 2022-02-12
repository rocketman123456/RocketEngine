#include "Vulkan/VulkanUtils.h"
// #include "Log/Instrumentor.h"
#include "Log/Log.h"

namespace Rocket {
    uint32_t GetVulkanBufferAlignment(VulkanRenderDevice& vkDev) {
        VkPhysicalDeviceProperties devProps;
        vkGetPhysicalDeviceProperties(vkDev.physical_device, &devProps);
        return static_cast<uint32_t>(devProps.limits.minStorageBufferOffsetAlignment);
    }

    // Check if the texture is used as a depth buffer
    bool IsDepthFormat(VkFormat fmt) {
        return
            (fmt == VK_FORMAT_D16_UNORM) ||
            (fmt == VK_FORMAT_X8_D24_UNORM_PACK32) ||
            (fmt == VK_FORMAT_D32_SFLOAT) ||
            (fmt == VK_FORMAT_D16_UNORM_S8_UINT) ||
            (fmt == VK_FORMAT_D24_UNORM_S8_UINT) ||
            (fmt == VK_FORMAT_D32_SFLOAT_S8_UINT);
    }

    bool SetVkObjectName(
        VulkanRenderDevice& vkDev, 
        void* object, 
        VkObjectType objType, 
        const std::string& name
    ) {
        VkDebugUtilsObjectNameInfoEXT nameInfo = {};
        nameInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
        nameInfo.pNext = nullptr;
        nameInfo.objectType = objType;
        nameInfo.objectHandle = (uint64_t)object;
        nameInfo.pObjectName = name.c_str();
        return (vkSetDebugUtilsObjectNameEXT(vkDev.device, &nameInfo) == VK_SUCCESS);
    }

    bool SetVkImageName(
        VulkanRenderDevice& vkDev, 
        void* object, 
        const std::string& name
    ) {
        return SetVkObjectName(vkDev, object, VK_OBJECT_TYPE_IMAGE, name);
    }

    VkResult CreateImageView(
        const VkDevice& device, 
        const VolkDeviceTable& table,
        const VkImage& image, 
        const VkFormat& format, 
        const VkImageAspectFlags& aspectFlags, 
        VkImageView* imageView, 
        VkImageViewType viewType, 
        uint32_t layerCount, 
        uint32_t mipLevels
    ) {
        VkImageViewCreateInfo viewInfo = {};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.pNext = nullptr;
        viewInfo.flags = 0;
        viewInfo.image = image;
        viewInfo.viewType = viewType;
        viewInfo.format = format;
        // TODO : check VK_COMPONENT_SWIZZLE_IDENTITY usage
        viewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        viewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        viewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        viewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        viewInfo.subresourceRange.aspectMask = aspectFlags;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = mipLevels;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = layerCount;

        return table.vkCreateImageView(device, &viewInfo, nullptr, imageView);
    }

    VkResult CreateSemaphore(
        const VkDevice& device, 
        const VolkDeviceTable& table,
        VkSemaphore* outSemaphore) {
        VkSemaphoreCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        return table.vkCreateSemaphore(device, &createInfo, nullptr, outSemaphore);
    }

    VkResult CreateFence(
        const VkDevice& device, 
        const VolkDeviceTable& table,
        VkFence* fence
    ) {
        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
        return table.vkCreateFence(device, &fenceInfo, nullptr, fence);
    }

    VkResult CreateCommandPool(
        const VkDevice& device, 
        const VolkDeviceTable& table, 
        const QueueFamilyIndices& indices, 
        VkCommandPool* command_pool
    ) {
        VkCommandPoolCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        createInfo.flags = 0;
        createInfo.queueFamilyIndex = indices.graphics_family.value();
        return table.vkCreateCommandPool(device, &createInfo, nullptr, command_pool);
    }

    VkResult CreateCommandBuffer(
        const VkDevice& device, 
        const VolkDeviceTable& table,
        const VkCommandPool& commandPool,
        uint32_t imageCount,
        VkCommandBuffer* command_buffer
    ) {
        VkCommandBufferAllocateInfo allocateInfo = {};
		allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocateInfo.pNext = nullptr;
		allocateInfo.commandPool = commandPool;
		allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocateInfo.commandBufferCount = imageCount;
        return table.vkAllocateCommandBuffers(device, &allocateInfo, command_buffer);
    }
}
