#include "Vulkan/VulkanDeviceUtils.h"
#include "Log/Log.h"

#include <set>

namespace Rocket {
    bool CheckDeviceExtensionSupport(
        const VkPhysicalDevice& device, 
        const std::vector<const char*>& deviceExtensions
    ) {
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

        std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());
        for (const auto& extension : requiredExtensions) {
            RK_INFO(Graphics, "Required Device Extension: {}", extension);
        }
        for (const auto& extension : availableExtensions) {
            requiredExtensions.erase(extension.extensionName);
        }
        for (const auto& extension : requiredExtensions) {
            RK_INFO(Graphics, "Not Available Device Extension: {}", extension);
        }

        return requiredExtensions.empty();
    }

    bool IsDeviceSuitable(
        const VkPhysicalDevice& device, 
        const VkSurfaceKHR& surface, 
        const std::vector<const char*>& deviceExtensions
    ) {
        QueueFamilyIndices indices = FindQueueFamilyIndices(device, surface);

        bool extensionsSupported = CheckDeviceExtensionSupport(device, deviceExtensions);
        bool swapChainAdequate = false;
        if (extensionsSupported) {
            auto swapChainSupport = QuerySwapchainSupport(device, surface);
            swapChainAdequate = !swapChainSupport.formats.empty() && 
                !swapChainSupport.presentModes.empty();
        }

        VkPhysicalDeviceFeatures supportedFeatures;
        vkGetPhysicalDeviceFeatures(device, &supportedFeatures);
        return indices.IsComplete() && 
            extensionsSupported && 
            swapChainAdequate && 
            supportedFeatures.samplerAnisotropy;
    }

    VkResult PickPhysicalDevice(
            const VkInstance& instance, 
            const VkSurfaceKHR& surface, 
            const std::vector<const char*>& deviceExtensions,
            VkPhysicalDevice* physical_device
    ) {
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
        if(deviceCount == 0) {
            RK_ERROR(Graphics, "failed to find GPUs with Vulkan support!");
            throw std::runtime_error("failed to find GPUs with Vulkan support!");
        }

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
        for(const auto& device : devices) {
            if (IsDeviceSuitable(device, surface, deviceExtensions)) {
                *physical_device = device;
                break;
            }
        }

        if(*physical_device == VK_NULL_HANDLE) {
            RK_ERROR(Graphics, "failed to find a suitable GPU!");
            return VK_ERROR_INITIALIZATION_FAILED;
        }
        return VK_SUCCESS;
    }

    QueueFamilyIndices FindQueueFamilyIndices(
        const VkPhysicalDevice& physical_device, 
        const VkSurfaceKHR& surface
    ) {
        QueueFamilyIndices indices;
        indices.present_family = FindPresentFamilies(physical_device, surface);
        indices.graphics_family = FindQueueFamilies(physical_device, VK_QUEUE_GRAPHICS_BIT);
        indices.compute_family = FindQueueFamilies(physical_device, VK_QUEUE_COMPUTE_BIT);
        return indices;
    }

    uint32_t FindQueueFamilies(
        const VkPhysicalDevice& device, 
        const VkQueueFlags& desiredFlags
    ) {
        uint32_t familyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &familyCount, nullptr);
        std::vector<VkQueueFamilyProperties> families(familyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &familyCount, families.data());

        for (uint32_t i = 0; i != families.size(); i++) {
            if (families[i].queueCount > 0 && families[i].queueFlags & desiredFlags) {
                return i;
            }
        }
        return 0;
    }

    uint32_t FindPresentFamilies(
        const VkPhysicalDevice& device, 
        const VkSurfaceKHR& surface
    ) {
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());
        int i = 0;
        for (const auto& queueFamily : queueFamilies) {
            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
            if (presentSupport) {
                return i;
            }
            i++;
        }
        return 0;
    }

    VkResult CreateLogicalDevice(
        VkPhysicalDevice physicalDevice, 
        VkPhysicalDeviceFeatures deviceFeatures, 
        const std::vector<const char*>& validationLayers,
        const std::vector<const char*>& extensions,
        uint32_t graphicsFamily, 
        VkDevice* device
    ) {
        const float queuePriority = 1.0f;

        VkDeviceQueueCreateInfo qci = {};
        qci.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        qci.pNext = nullptr;
        qci.flags = 0;
        qci.queueFamilyIndex = graphicsFamily;
        qci.queueCount = 1;
        qci.pQueuePriorities = &queuePriority;

        VkDeviceCreateInfo ci = {};
        ci.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        ci.pNext = nullptr;
        ci.flags = 0;
        ci.queueCreateInfoCount = 1;
        ci.pQueueCreateInfos = &qci;
        ci.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        ci.ppEnabledLayerNames = validationLayers.data();
        ci.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        ci.ppEnabledExtensionNames = extensions.data();
        ci.pEnabledFeatures = &deviceFeatures;

        return vkCreateDevice(physicalDevice, &ci, nullptr, device);
    }

    VkResult CreateLogicalDeviceWithCompute(
        VkPhysicalDevice physicalDevice, 
        VkPhysicalDeviceFeatures deviceFeatures, 
        const std::vector<const char*>& validationLayers,
        const std::vector<const char*>& extensions,
        uint32_t graphicsFamily, 
        uint32_t computeFamily, 
        VkDevice* device
    ) {
        if (graphicsFamily == computeFamily)
            return CreateLogicalDevice(
                physicalDevice, deviceFeatures, 
                validationLayers, extensions, 
                graphicsFamily, device
            );

        const float queuePriorities[2] = { 0.f, 0.f };
        VkDeviceQueueCreateInfo qciGfx = {};
        qciGfx.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        qciGfx.pNext = nullptr;
        qciGfx.flags = 0;
        qciGfx.queueFamilyIndex = graphicsFamily;
        qciGfx.queueCount = 1;
        qciGfx.pQueuePriorities = &queuePriorities[0];

        VkDeviceQueueCreateInfo qciComp = {};
        qciComp.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        qciComp.pNext = nullptr;
        qciComp.flags = 0;
        qciComp.queueFamilyIndex = computeFamily;
        qciComp.queueCount = 1;
        qciComp.pQueuePriorities = &queuePriorities[1];

        const VkDeviceQueueCreateInfo qci[2] = { qciGfx, qciComp };

        VkDeviceCreateInfo ci = {};
        ci.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        ci.pNext = nullptr;
        ci.flags = 0;
        ci.queueCreateInfoCount = 2;
        ci.pQueueCreateInfos = qci;
        ci.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        ci.ppEnabledLayerNames = validationLayers.data();
        ci.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        ci.ppEnabledExtensionNames = extensions.data();
        ci.pEnabledFeatures = &deviceFeatures;

        return vkCreateDevice(physicalDevice, &ci, nullptr, device);
    }

    VkResult CreateLogicalDevice(
        const VkPhysicalDevice& physicalDevice, 
        const std::vector<const char*>& validationLayers,
        const std::vector<const char*>& deviceExtensions, 
        const QueueFamilyIndices& indices,
        VkDevice* device
    ) {
        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<uint32_t> uniqueQueueFamilies = {
            indices.graphics_family.value(),
            indices.compute_family.value(),
            indices.present_family.value(),
        };

        float queuePriority = 1.0f;
        for(auto queueFamily : uniqueQueueFamilies) {
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        VkPhysicalDeviceFeatures deviceFeatures{};
        // deviceFeatures.geometryShader = VK_TRUE;
        // deviceFeatures.tessellationShader = VK_TRUE;
        deviceFeatures.samplerAnisotropy = VK_TRUE;

        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        createInfo.pQueueCreateInfos = queueCreateInfos.data();
        createInfo.pEnabledFeatures = &deviceFeatures;
        createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
        createInfo.ppEnabledExtensionNames = deviceExtensions.data();
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();

        return vkCreateDevice(physicalDevice, &createInfo, nullptr, device);
    }

    SwapchainSupportDetails QuerySwapchainSupport(
        const VkPhysicalDevice& device, 
        const VkSurfaceKHR& surface
    ) {
        SwapchainSupportDetails details;
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
        if (formatCount) {
            details.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
        }

        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);
        if (presentModeCount) {
            details.presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
        }
        return details;
    }

    VkSurfaceFormatKHR ChooseSwapSurfaceFormat(
        const std::vector<VkSurfaceFormatKHR>& availableFormats
    ) {
        return { VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
    }

    VkPresentModeKHR ChooseSwapPresentMode(
        const std::vector<VkPresentModeKHR>& availablePresentModes
    ) {
        for (const auto mode : availablePresentModes)
            if (mode == VK_PRESENT_MODE_MAILBOX_KHR)
                return mode;
        // FIFO will always be supported
        return VK_PRESENT_MODE_FIFO_KHR;
    }

    uint32_t ChooseSwapImageCount(const VkSurfaceCapabilitiesKHR& capabilities) {
        const uint32_t imageCount = capabilities.minImageCount + 1;
        const bool imageCountExceeded = capabilities.maxImageCount > 0 && 
            imageCount > capabilities.maxImageCount;
        return imageCountExceeded ? capabilities.maxImageCount : imageCount;
    }

    VkExtent2D ChooseSwapExtent(
        const VkSurfaceCapabilitiesKHR& capabilities, 
        uint32_t width, uint32_t height
    ) {
        if (capabilities.currentExtent.width != UINT32_MAX) {
            return capabilities.currentExtent;
        } else {
            VkExtent2D actualExtent = { width, height};
            actualExtent.width = std::clamp(
                actualExtent.width, 
                capabilities.minImageExtent.width, 
                capabilities.maxImageExtent.width);
            actualExtent.height = std::clamp(
                actualExtent.height, 
                capabilities.minImageExtent.height, 
                capabilities.maxImageExtent.height);
            return actualExtent;
        }
    }

    VkResult CreateSwapchain(
        const VkPhysicalDevice& physicalDevice, 
        const VkDevice& device,
        const VolkDeviceTable& table,
        const VkSurfaceKHR& surface, 
        const QueueFamilyIndices& indices, 
        VkFormat* swapChainImageFormat,
        VkSwapchainKHR* swapchain,
        uint32_t width, 
        uint32_t height, 
        bool supportScreenshots
    ) {
        auto swapchainSupport = QuerySwapchainSupport(physicalDevice, surface);
        auto surfaceFormat = ChooseSwapSurfaceFormat(swapchainSupport.formats);
        auto presentMode = ChooseSwapPresentMode(swapchainSupport.presentModes);
        auto imageCount = ChooseSwapImageCount(swapchainSupport.capabilities);
        auto extent = ChooseSwapExtent(swapchainSupport.capabilities, width, height);
        *swapChainImageFormat = surfaceFormat.format;

        VkSwapchainCreateInfoKHR createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.flags = 0;
        createInfo.surface = surface;
        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = 
            VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | 
            VK_IMAGE_USAGE_TRANSFER_DST_BIT | 
            (supportScreenshots ? VK_IMAGE_USAGE_TRANSFER_SRC_BIT : 0u);
        
        std::set<uint32_t> queueFamilyIndicesCount = {
            indices.graphics_family.value(),
            indices.present_family.value(),
            indices.compute_family.value()
        };

        std::vector<uint32_t> queueFamilyIndices(
            queueFamilyIndicesCount.begin(), 
            queueFamilyIndicesCount.end()
        );

        if (queueFamilyIndicesCount.size() > 1) {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        } else {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        }

        createInfo.queueFamilyIndexCount = static_cast<uint32_t>(queueFamilyIndices.size());
        createInfo.pQueueFamilyIndices = queueFamilyIndices.data();
        createInfo.preTransform = swapchainSupport.capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;
        createInfo.oldSwapchain = VK_NULL_HANDLE;

        return table.vkCreateSwapchainKHR(device, &createInfo, nullptr, swapchain);
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
        viewInfo.subresourceRange.aspectMask = aspectFlags;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = mipLevels;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = layerCount;

        return table.vkCreateImageView(device, &viewInfo, nullptr, imageView);
    }

    std::size_t CreateSwapchainImages(
        const VkDevice& device, 
        const VolkDeviceTable& table,
        const VkSwapchainKHR& swapchain,
        const VkFormat& swapChainImageFormat,
        std::vector<VkImage>* swapchainImages,
        std::vector<VkImageView>* swapchainImageViews
    ) {
        uint32_t imageCount = 0;
        VK_CHECK(table.vkGetSwapchainImagesKHR(device, swapchain, &imageCount, nullptr));

        swapchainImages->resize(imageCount);
        swapchainImageViews->resize(imageCount);

        VK_CHECK(table.vkGetSwapchainImagesKHR(device, swapchain, &imageCount, swapchainImages->data()));

        for (unsigned i = 0; i < imageCount; i++) {
            VK_CHECK(CreateImageView(
                device, table,
                (*swapchainImages)[i], 
                swapChainImageFormat,
                VK_IMAGE_ASPECT_COLOR_BIT, 
                &(*swapchainImageViews)[i]));
        }

        return static_cast<std::size_t>(imageCount);
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

    bool InitVulkanRenderDevice(
        VulkanInstance& vk, 
        VulkanRenderDevice& vkDev, 
        const std::vector<const char*>& deviceExtensions,
        const std::vector<const char*>& validationLayers,
        uint32_t width, 
        uint32_t height
    ) {
        vkDev.framebuffer_width = width;
        vkDev.framebuffer_height = height;

        VK_CHECK(PickPhysicalDevice(vk.instance, vk.surface, deviceExtensions, &vkDev.physical_device));
        vkDev.family = FindQueueFamilyIndices(vkDev.physical_device, vk.surface);
        VK_CHECK(CreateLogicalDevice(vkDev.physical_device,validationLayers, deviceExtensions, 
            vkDev.family, &vkDev.device));

        volkLoadDeviceTable(&vkDev.table, vkDev.device);

        vkDev.table.vkGetDeviceQueue(vkDev.device, vkDev.family.present_family.value(), 0, &vkDev.present_queue);
        if (vkDev.present_queue == nullptr) {
            RK_ERROR(Graphics, "Failed to Get present_queue!");
            exit(EXIT_FAILURE);
        }
        vkDev.table.vkGetDeviceQueue(vkDev.device, vkDev.family.graphics_family.value(), 0, &vkDev.graphics_queue);
        if (vkDev.graphics_queue == nullptr) {
            RK_ERROR(Graphics, "Failed to Get graphics_queue!");
            exit(EXIT_FAILURE);
        }
        vkDev.table.vkGetDeviceQueue(vkDev.device, vkDev.family.compute_family.value(), 0, &vkDev.compute_queue);
        if (vkDev.compute_queue == nullptr) {
            RK_ERROR(Graphics, "Failed to Get compute_queue!");
            exit(EXIT_FAILURE);
        }
        vkDev.device_queue_indices = vkDev.family.FamilyData();

        VkBool32 presentSupported = 0;
        vkGetPhysicalDeviceSurfaceSupportKHR(
            vkDev.physical_device, vkDev.family.graphics_family.value(), vk.surface, &presentSupported);
        if (!presentSupported) {
            RK_ERROR(Graphics, "Present Not Supported!");
            exit(EXIT_FAILURE);
        }

        VK_CHECK(CreateSwapchain(
            vkDev.physical_device, vkDev.device, vkDev.table ,vk.surface, 
            vkDev.family, &vkDev.swapchain_image_format, &vkDev.swapchain, width, height));
        const size_t imageCount = CreateSwapchainImages(
            vkDev.device, vkDev.table, vkDev.swapchain, vkDev.swapchain_image_format, 
            &vkDev.swapchain_images, &vkDev.swapchain_image_views);
        vkDev.command_buffers.resize(imageCount);

        VK_CHECK(CreateSemaphore(vkDev.device, vkDev.table, &vkDev.semaphore));
        VK_CHECK(CreateSemaphore(vkDev.device, vkDev.table, &vkDev.render_semaphore));

        auto frame_count = vkDev.swapchain_images.size();
        vkDev.image_available_semaphores.resize(frame_count);
        vkDev.render_finish_semaphores.resize(frame_count);
        vkDev.in_flight_fences.resize(frame_count);
        vkDev.images_in_flight.resize(frame_count, VK_NULL_HANDLE);
        for(int i = 0; i < frame_count; ++i) {
            VK_CHECK(CreateSemaphore(vkDev.device, vkDev.table, &vkDev.image_available_semaphores[i]));
            VK_CHECK(CreateSemaphore(vkDev.device, vkDev.table, &vkDev.render_finish_semaphores[i]));
            VK_CHECK(CreateFence(vkDev.device, vkDev.table, &vkDev.in_flight_fences[i]));
        }

        {
            // Create graphics command pool
            VkCommandPoolCreateInfo cpi = {};
            cpi.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
            cpi.flags = 0;
            cpi.queueFamilyIndex = vkDev.family.graphics_family.value();

            VK_CHECK(vkDev.table.vkCreateCommandPool(vkDev.device, &cpi, nullptr, &vkDev.command_pool));

            VkCommandBufferAllocateInfo ai = {};
            ai.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            ai.pNext = nullptr;
            ai.commandPool = vkDev.command_pool;
            ai.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
            ai.commandBufferCount = static_cast<uint32_t>(vkDev.swapchain_images.size());

            VK_CHECK(vkDev.table.vkAllocateCommandBuffers(vkDev.device, &ai, vkDev.command_buffers.data()));
        }

        {
            // Create compute command pool
            VkCommandPoolCreateInfo cpi = {};
            cpi.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
            cpi.pNext = nullptr;
            /* Allow command from this pool buffers to be reset*/
            cpi.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT; 
            cpi.queueFamilyIndex = vkDev.family.compute_family.value();
            VK_CHECK(vkDev.table.vkCreateCommandPool(vkDev.device, &cpi, nullptr, &vkDev.compute_command_pool));

            VkCommandBufferAllocateInfo ai = {};
            ai.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            ai.pNext = nullptr;
            ai.commandPool = vkDev.compute_command_pool;
            ai.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
            ai.commandBufferCount = 1;

            VK_CHECK(vkDev.table.vkAllocateCommandBuffers(vkDev.device, &ai, &vkDev.compute_command_buffer));
        }
        return true;
    }

    void CleanupVulkanRenderDevice(VulkanRenderDevice& vkDev) {
        vkDev.table.vkDestroyCommandPool(vkDev.device, vkDev.compute_command_pool, nullptr);
        vkDev.table.vkDestroyCommandPool(vkDev.device, vkDev.command_pool, nullptr);
        for(auto semaphore : vkDev.image_available_semaphores) {
            vkDev.table.vkDestroySemaphore(vkDev.device, semaphore, nullptr);
        }
        for(auto semaphore : vkDev.render_finish_semaphores) {
            vkDev.table.vkDestroySemaphore(vkDev.device, semaphore, nullptr);
        }
        for(auto fence : vkDev.in_flight_fences) {
            vkDev.table.vkDestroyFence(vkDev.device, fence, nullptr);
        }
        vkDev.table.vkDestroySemaphore(vkDev.device, vkDev.render_semaphore, nullptr);
        vkDev.table.vkDestroySemaphore(vkDev.device, vkDev.semaphore, nullptr);
        for (auto image_view : vkDev.swapchain_image_views) {
            vkDev.table.vkDestroyImageView(vkDev.device, image_view, nullptr);
        }
        vkDev.table.vkDestroySwapchainKHR(vkDev.device, vkDev.swapchain, nullptr);
        vkDev.table.vkDestroyDevice(vkDev.device, nullptr);
        RK_INFO(Graphics, "Finalize Vulkan Device");
    }

//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------

    VkFormat FindSupportedFormat(
        const VkPhysicalDevice& device, 
        const std::vector<VkFormat>& candidates, 
        VkImageTiling tiling, 
        VkFormatFeatureFlags features
    ) {
        for (VkFormat format : candidates) {
            VkFormatProperties props;
            vkGetPhysicalDeviceFormatProperties(device, format, &props);

            if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
                return format;
            }
            else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
                return format;
            }
        }
        RK_ERROR(Graphics, "failed to find supported format!");
        exit(0);
    }

    uint32_t FindMemoryType(
        const VkPhysicalDevice& device, 
        uint32_t typeFilter, 
        VkMemoryPropertyFlags properties
    ) {
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(device, &memProperties);

        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
            if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                return i;
            }
        }
        return 0xFFFFFFFF;
    }

    VkFormat FindDepthFormat(const VkPhysicalDevice& device) {
        return FindSupportedFormat(
            device,
            { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
            VK_IMAGE_TILING_OPTIMAL,
            VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
        );
    }

    bool HasStencilComponent(VkFormat format) {
        return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
    }

//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------

    VkCommandBuffer BeginSingleTimeCommands(VulkanRenderDevice& vkDev) {
        VkCommandBuffer commandBuffer;

        VkCommandBufferAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.pNext = nullptr;
        allocInfo.commandPool = vkDev.command_pool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = 1;

        VK_CHECK(vkDev.table.vkAllocateCommandBuffers(vkDev.device, &allocInfo, &commandBuffer));

        VkCommandBufferBeginInfo beginInfo = {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.pNext = nullptr;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        beginInfo.pInheritanceInfo = nullptr;

        VK_CHECK(vkDev.table.vkBeginCommandBuffer(commandBuffer, &beginInfo));
        return commandBuffer;
    }

    void EndSingleTimeCommands(VulkanRenderDevice& vkDev, VkCommandBuffer commandBuffer) {
        VK_CHECK(vkDev.table.vkEndCommandBuffer(commandBuffer));

        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.pNext = nullptr;
        submitInfo.waitSemaphoreCount = 0;
        submitInfo.pWaitSemaphores = nullptr;
        submitInfo.pWaitDstStageMask = nullptr;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;
        submitInfo.signalSemaphoreCount = 0;
        submitInfo.pSignalSemaphores = nullptr;

        VK_CHECK(vkDev.table.vkQueueSubmit(vkDev.graphics_queue, 1, &submitInfo, VK_NULL_HANDLE));
        VK_CHECK(vkDev.table.vkQueueWaitIdle(vkDev.graphics_queue));
        vkDev.table.vkFreeCommandBuffers(vkDev.device, vkDev.command_pool, 1, &commandBuffer);
    }

//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------

    bool CreateImage(
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
        return true;
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

//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------

    bool CreateDepthResources(VulkanRenderDevice& vkDev, uint32_t width, uint32_t height, VulkanImage& depth) {
        VkFormat depthFormat = FindDepthFormat(vkDev.physical_device);

        if (!CreateImage(
            vkDev.device, 
            vkDev.physical_device, 
            vkDev.table, 
            width, height, depthFormat, 
            VK_IMAGE_TILING_OPTIMAL, 
            VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, 
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
            depth.image, 
            depth.image_memory))
            return false;

        if (!CreateImageView(
            vkDev.device, vkDev.table, depth.image, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, &depth.image_view))
            return false;

        TransitionImageLayout(
            vkDev, depth.image, depthFormat, 
            VK_IMAGE_LAYOUT_UNDEFINED, 
            VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

        return true;
    }

//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------

    VkResult CreateDescriptorPool(
        VulkanRenderDevice& vkDev, 
        uint32_t uniformBufferCount, 
        uint32_t storageBufferCount, 
        uint32_t samplerCount, 
        VkDescriptorPool* descriptorPool
    ) {
        const uint32_t imageCount = static_cast<uint32_t>(vkDev.swapchain_images.size());
        std::vector<VkDescriptorPoolSize> poolSizes;

        if (uniformBufferCount) {
            VkDescriptorPoolSize poolSize = {};
            poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            poolSize.descriptorCount = imageCount * uniformBufferCount;
            poolSizes.push_back(poolSize);
        }
        if (storageBufferCount) {
            VkDescriptorPoolSize poolSize = {};
            poolSize.type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
            poolSize.descriptorCount = imageCount * storageBufferCount;
            poolSizes.push_back(poolSize);
        }
        if (samplerCount) {
            VkDescriptorPoolSize poolSize = {};
            poolSize.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            poolSize.descriptorCount = imageCount * samplerCount;
            poolSizes.push_back(poolSize);
        }

        VkDescriptorPoolCreateInfo poolInfo = {};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.pNext = nullptr;
        poolInfo.flags = 0;
        poolInfo.maxSets = static_cast<uint32_t>(imageCount);
        poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
        poolInfo.pPoolSizes = poolSizes.empty() ? nullptr : poolSizes.data();
        return vkDev.table.vkCreateDescriptorPool(vkDev.device, &poolInfo, nullptr, descriptorPool);
    }

    VkDescriptorSetLayoutBinding DescriptorSetLayoutBinding(
        uint32_t binding, 
        VkDescriptorType descriptorType, 
        VkShaderStageFlags stageFlags, 
        uint32_t descriptorCount
    ) {
        VkDescriptorSetLayoutBinding binding_ = {};
        binding_.binding = binding;
        binding_.descriptorType = descriptorType;
        binding_.descriptorCount = descriptorCount;
        binding_.stageFlags = stageFlags;
        binding_.pImmutableSamplers = nullptr;
        return binding_;
    }

    bool CreateDescriptorSet(
        VulkanRenderDevice& vkDev, 
        VulkanState* vkState,
        uint32_t uniformBufferSize,
        uint32_t vertexBufferSize,
        uint32_t indexBufferSize
    ) {
        const std::array<VkDescriptorSetLayoutBinding, 4> bindings = {
            DescriptorSetLayoutBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT),
            DescriptorSetLayoutBinding(1, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, VK_SHADER_STAGE_VERTEX_BIT),
            DescriptorSetLayoutBinding(2, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, VK_SHADER_STAGE_VERTEX_BIT),
            DescriptorSetLayoutBinding(3, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
        };

        VkDescriptorSetLayoutCreateInfo layoutInfo = {};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.pNext = nullptr;
        layoutInfo.flags = 0;
        layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
        layoutInfo.pBindings = bindings.data();

        VK_CHECK(vkDev.table.vkCreateDescriptorSetLayout(vkDev.device, &layoutInfo, nullptr, &vkState->descriptor_set_layout));

        std::vector<VkDescriptorSetLayout> layouts(vkDev.swapchain_images.size(), vkState->descriptor_set_layout);

        VkDescriptorSetAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.pNext = nullptr;
        allocInfo.descriptorPool = vkState->descriptor_pool;
        allocInfo.descriptorSetCount = static_cast<uint32_t>(vkDev.swapchain_images.size());
        allocInfo.pSetLayouts = layouts.data();

        vkState->descriptor_sets.resize(vkDev.swapchain_images.size());
        VK_CHECK(vkDev.table.vkAllocateDescriptorSets(vkDev.device, &allocInfo, vkState->descriptor_sets.data()));

        for (size_t i = 0; i < vkDev.swapchain_images.size(); i++) {
            // VkDescriptorBufferInfo bufferInfo = {};
            // bufferInfo.buffer = vkState->uniform_buffers[i];
            // bufferInfo.offset = 0;
            // bufferInfo.range = uniformBufferSize;
            // VkDescriptorBufferInfo bufferInfo2 = {};
            // bufferInfo2.buffer = vkState->storage_buffer;
            // bufferInfo2.offset = 0;
            // bufferInfo2.range = vertexBufferSize;
            // VkDescriptorBufferInfo bufferInfo3 = {};
            // bufferInfo3.buffer = vkState->storage_buffer;
            // bufferInfo3.offset = vertexBufferSize;
            // bufferInfo3.range = indexBufferSize;
            // VkDescriptorImageInfo imageInfo = {};
            // imageInfo.sampler = vkState->texture_sampler;
            // imageInfo.imageView = vkState->texture.image_view;
            // imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

            // VkWriteDescriptorSet set0 = {};
            // set0.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            // set0.dstSet = vkState->descriptor_sets[i];
            // set0.dstBinding = 0;
            // set0.dstArrayElement = 0;
            // set0.descriptorCount = 1;
            // set0.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            // set0.pBufferInfo = &bufferInfo;
            // VkWriteDescriptorSet set1 = {};
            // set1.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            // set1.dstSet = vkState->descriptor_sets[i];
            // set1.dstBinding = 1;
            // set1.dstArrayElement = 0;
            // set1.descriptorCount = 1;
            // set1.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
            // set1.pBufferInfo = &bufferInfo2;
            // VkWriteDescriptorSet set2 = {};
            // set2.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            // set2.dstSet = vkState->descriptor_sets[i];
            // set2.dstBinding = 2;
            // set2.dstArrayElement = 0;
            // set2.descriptorCount = 1;
            // set2.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
            // set2.pBufferInfo = &bufferInfo3;
            // VkWriteDescriptorSet set3 = {};
            // set3.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            // set3.dstSet = vkState->descriptor_sets[i];
            // set3.dstBinding = 3;
            // set3.dstArrayElement = 0;
            // set3.descriptorCount = 1;
            // set3.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            // set3.pImageInfo = &imageInfo;

            // const std::array<VkWriteDescriptorSet, 4> descriptorWrites = {
            //     set0, set1, set2, set3,
            // };

            // vkDev.table.vkUpdateDescriptorSets(vkDev.device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
        }

        return true;
    }
}
