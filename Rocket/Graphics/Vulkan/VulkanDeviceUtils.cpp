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

    QueueFamilyIndices FindQueueFamilyIndices(VkPhysicalDevice physical_device, VkSurfaceKHR surface) {
        QueueFamilyIndices indices;
        indices.present_family = FindPresentFamilies(physical_device, surface);
        indices.graphics_family = FindQueueFamilies(physical_device, VK_QUEUE_GRAPHICS_BIT);
        indices.compute_family = FindQueueFamilies(physical_device, VK_QUEUE_COMPUTE_BIT);
        return indices;
    }

    uint32_t FindQueueFamilies(VkPhysicalDevice device, VkQueueFlags desiredFlags) {
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

    uint32_t FindPresentFamilies(VkPhysicalDevice device, VkSurfaceKHR surface) {
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
        VkSemaphoreCreateInfo ci = {};
        ci.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        return table.vkCreateSemaphore(device, &ci, nullptr, outSemaphore);
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
        vkDev.table.vkDestroySemaphore(vkDev.device, vkDev.render_semaphore, nullptr);
        vkDev.table.vkDestroySemaphore(vkDev.device, vkDev.semaphore, nullptr);
        for (auto image_view : vkDev.swapchain_image_views) {
            vkDev.table.vkDestroyImageView(vkDev.device, image_view, nullptr);
        }
        vkDev.table.vkDestroySwapchainKHR(vkDev.device, vkDev.swapchain, nullptr);
        vkDev.table.vkDestroyDevice(vkDev.device, nullptr);
        RK_INFO(Graphics, "Finalize Vulkan Device");
    }
}
