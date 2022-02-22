#include "Vulkan/VulkanImageUtils.h"
#include "Vulkan/VulkanDeviceUtils.h"

namespace Rocket {
    VkResult CreateImage(
        const VkDevice& device, 
        const VkPhysicalDevice& physicalDevice, 
        const VolkDeviceTable& table, 
        uint32_t width, 
        uint32_t height, 
        VkFormat format, 
        VkImageTiling tiling, 
        VkImageUsageFlags usage, 
        VkMemoryPropertyFlags properties, 
        VkImage& image, 
        VkDeviceMemory& imageMemory, 
        VkImageCreateFlags flags, 
        uint32_t mipLevels
    ) {
        VkImageCreateInfo imageInfo = {};
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.pNext = nullptr;
        imageInfo.flags = flags;
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.format = format;
        imageInfo.extent.width = width;
        imageInfo.extent.height = height;
        imageInfo.extent.depth = 1;
        imageInfo.mipLevels = mipLevels;
        imageInfo.arrayLayers = (uint32_t)((flags == VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT) ? 6 : 1);
        imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        imageInfo.tiling = tiling;
        imageInfo.usage = usage;
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        imageInfo.queueFamilyIndexCount = 0;
        imageInfo.pQueueFamilyIndices = nullptr;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

        VK_CHECK(table.vkCreateImage(device, &imageInfo, nullptr, &image));

        VkMemoryRequirements memRequirements;
        table.vkGetImageMemoryRequirements(device, image, &memRequirements);

        VkMemoryAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.pNext = nullptr;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = FindMemoryType(physicalDevice, memRequirements.memoryTypeBits, properties);

        VK_CHECK(table.vkAllocateMemory(device, &allocInfo, nullptr, &imageMemory));

        table.vkBindImageMemory(device, image, imageMemory, 0);
        return VK_SUCCESS;
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
        // viewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        // viewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        // viewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        // viewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        viewInfo.subresourceRange.aspectMask = aspectFlags;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = mipLevels;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = layerCount;

        return table.vkCreateImageView(device, &viewInfo, nullptr, imageView);
    }

    VkResult CreateTextureSampler(
        const VkDevice& device, 
        const VkPhysicalDevice& physicalDevice, 
        const VolkDeviceTable& table,
        VkSampler* textureSampler,
        const VkFilter& minFilter, 
        const VkFilter& maxFilter, 
        const VkSamplerAddressMode& addressMode
    ) {
        VkPhysicalDeviceProperties properties{};
        vkGetPhysicalDeviceProperties(physicalDevice, &properties);

        VkSamplerCreateInfo samplerInfo{};
        samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerInfo.minFilter = minFilter;
        samplerInfo.magFilter = maxFilter;
        samplerInfo.addressModeU = addressMode;
        samplerInfo.addressModeV = addressMode;
        samplerInfo.addressModeW = addressMode;
        samplerInfo.anisotropyEnable = VK_TRUE;
        samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
        samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        samplerInfo.unnormalizedCoordinates = VK_FALSE;
        samplerInfo.compareEnable = VK_FALSE;
        samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
        samplerInfo.minLod = 0.0f,
		samplerInfo.maxLod = 0.0f,
        samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;

        return table.vkCreateSampler(device, &samplerInfo, nullptr, textureSampler);
    }

    void TransitionImageLayout(
        VulkanRenderDevice& vkDev, 
        VkImage image, 
        VkFormat format, 
        VkImageLayout oldLayout, 
        VkImageLayout newLayout, 
        uint32_t layerCount, 
        uint32_t mipLevels
    ) {
        VkCommandBuffer commandBuffer = BeginSingleTimeCommands(vkDev);
        TransitionImageLayoutCmd(vkDev.table, commandBuffer, image, format, oldLayout, newLayout, layerCount, mipLevels);
        EndSingleTimeCommands(vkDev, commandBuffer);
    }

    void TransitionImageLayoutCmd(
        const VolkDeviceTable& table, 
        VkCommandBuffer commandBuffer, 
        VkImage image, 
        VkFormat format, 
        VkImageLayout oldLayout, 
        VkImageLayout newLayout, 
        uint32_t layerCount, 
        uint32_t mipLevels
    ) {
        VkImageMemoryBarrier barrier = {};
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.pNext = nullptr;
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = 0;
        barrier.oldLayout = oldLayout;
        barrier.newLayout = newLayout;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.image = image;
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.levelCount = mipLevels;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = layerCount;

        VkPipelineStageFlags sourceStage;
        VkPipelineStageFlags destinationStage;

        if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL ||
            (format == VK_FORMAT_D16_UNORM) ||
            (format == VK_FORMAT_X8_D24_UNORM_PACK32) ||
            (format == VK_FORMAT_D32_SFLOAT) ||
            (format == VK_FORMAT_S8_UINT) ||
            (format == VK_FORMAT_D16_UNORM_S8_UINT) ||
            (format == VK_FORMAT_D24_UNORM_S8_UINT)) {
            barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
            if (HasStencilComponent(format)) {
                barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
            }
        } else {
            barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        }

        if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
            barrier.srcAccessMask = 0;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        } else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_GENERAL) {
            barrier.srcAccessMask = 0;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
            destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        }
        if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
            barrier.srcAccessMask = 0;
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

            sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        }
        /* Convert back from read-only to updateable */
        else if (oldLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
            barrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

            sourceStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
            destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        }
        /* Convert from updateable texture to shader read-only */
        else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
            destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        }
        /* Convert depth texture from undefined state to depth-stencil buffer */
        else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
            barrier.srcAccessMask = 0;
            barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

            sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        }
        /* Wait for render pass to complete */
        else if (oldLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
            barrier.srcAccessMask = 0; // VK_ACCESS_SHADER_READ_BIT;
            barrier.dstAccessMask = 0;
    /*
            sourceStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    ///		destinationStage = VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT;
            destinationStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    */
            sourceStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        }
        /* Convert back from read-only to color attachment */
        else if (oldLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL) {
            barrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
            barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

            sourceStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
            destinationStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        }
        /* Convert from updateable texture to shader read-only */
        else if (oldLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
            barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            sourceStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        }
        /* Convert back from read-only to depth attachment */
        else if (oldLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
            barrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
            barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

            sourceStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
            destinationStage = VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
        }
        /* Convert from updateable depth texture to shader read-only */
        else if (oldLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
            barrier.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            sourceStage = VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
            destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        }

        table.vkCmdPipelineBarrier(
            commandBuffer,
            sourceStage, destinationStage,
            0,
            0, nullptr,
            0, nullptr,
            1, &barrier
        );
    }
}
