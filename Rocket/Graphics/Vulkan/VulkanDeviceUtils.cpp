#include "Vulkan/VulkanDeviceUtils.h"
#include "Vulkan/VulkanShaderUtils.h"
#include "Vulkan/VulkanImageUtils.h"
#include "Vulkan/VulkanUtils.h"
#include "Utils/FindRootDir.h"
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
        indices.PostProcess();
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
        float queuePriority = 1.0f;
        for(auto queueFamily : indices.unique_family_data) {
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

    VkResult GetVulkanQueue(
        const VkDevice& device,
        const VolkDeviceTable& table,
        uint32_t index,
        VkQueue* queue
    ) {
        table.vkGetDeviceQueue(device, index, 0, queue);
        if (queue == nullptr) {
            RK_ERROR(Graphics, "Failed to Get present_queue!");
            return VK_ERROR_INITIALIZATION_FAILED;
        }
        return VK_SUCCESS;
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
        VkExtent2D* swapchainExtent,
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
        *swapchainExtent = extent;

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

        if (indices.multiplicity.value() > 1) {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        } else {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        }

        createInfo.queueFamilyIndexCount = static_cast<uint32_t>(indices.unique_family_data.size());
        createInfo.pQueueFamilyIndices = indices.unique_family_data.data();
        createInfo.preTransform = swapchainSupport.capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;
        // TODO : use for recreate swap chain
        createInfo.oldSwapchain = VK_NULL_HANDLE;

        return table.vkCreateSwapchainKHR(device, &createInfo, nullptr, swapchain);
    }

    std::size_t CreateSwapchainImages(
        const VkDevice& device, 
        const VolkDeviceTable& table,
        const VkSwapchainKHR& swapchain,
        const VkFormat& swapChainImageFormat,
        std::vector<VkImage>* swapchainImages,
        std::vector<VkImageView>* swapchainImageViews
    ) {
        // Retrive Swap Chain Image
        uint32_t imageCount = 0;
        VK_CHECK(table.vkGetSwapchainImagesKHR(device, swapchain, &imageCount, nullptr));
        swapchainImages->resize(imageCount);
        VK_CHECK(table.vkGetSwapchainImagesKHR(device, swapchain, &imageCount, swapchainImages->data()));
        // Create Image Views
        swapchainImageViews->resize(swapchainImages->size());
        for (unsigned i = 0; i < imageCount; i++) {
            VK_CHECK(CreateImageView(
                device, table,
                (*swapchainImages)[i], swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT, 
                &(*swapchainImageViews)[i]
            ));
        }
        return static_cast<std::size_t>(imageCount);
    }

    VkResult InitVulkanRenderDevice(
        VulkanInstance& vk, 
        VulkanRenderDevice& vkDev, 
        const std::vector<const char*>& deviceExtensions,
        const std::vector<const char*>& validationLayers,
        uint32_t framebuffer_width, 
        uint32_t framebuffer_height
    ) {
        vkDev.framebuffer_width = framebuffer_width;
        vkDev.framebuffer_height = framebuffer_height;

        VK_CHECK(PickPhysicalDevice(vk.instance, vk.surface, deviceExtensions, &vkDev.physical_device));
        vkDev.family = FindQueueFamilyIndices(vkDev.physical_device, vk.surface);
        VK_CHECK(CreateLogicalDevice(vkDev.physical_device,validationLayers, deviceExtensions, vkDev.family, &vkDev.device));

        volkLoadDeviceTable(&vkDev.table, vkDev.device);

        // Find Different Queue for Different Type of Command
        VK_CHECK(GetVulkanQueue(vkDev.device, vkDev.table, vkDev.family.present_family.value(), &vkDev.present_queue));
        VK_CHECK(GetVulkanQueue(vkDev.device, vkDev.table, vkDev.family.graphics_family.value(), &vkDev.graphics_queue));
        VK_CHECK(GetVulkanQueue(vkDev.device, vkDev.table, vkDev.family.compute_family.value(), &vkDev.compute_queue));

        // Get All Device Queues
        vkDev.device_queue_indices = vkDev.family.unique_family_data;
        std::set<VkQueue> uniqueDeviceQueues = {
            vkDev.present_queue, vkDev.graphics_queue, vkDev.compute_queue
        };
        vkDev.device_queues.insert(vkDev.device_queues.begin(), uniqueDeviceQueues.begin(), uniqueDeviceQueues.end());

        // Check Swap Chain Support
        VkBool32 presentSupport = 0;
        vkGetPhysicalDeviceSurfaceSupportKHR(vkDev.physical_device, vkDev.family.graphics_family.value(), vk.surface, &presentSupport);
        if (!presentSupport) {
            RK_ERROR(Graphics, "Present Not Supported!");
            throw std::runtime_error("Present Not Supported!");
        }

        // Create Swap Chain
        VK_CHECK(CreateSwapchain(
            vkDev.physical_device, vkDev.device, vkDev.table ,vk.surface, vkDev.family, 
            &vkDev.swapchain_image_format, &vkDev.swapchain_extent, &vkDev.swapchain, 
            framebuffer_width, framebuffer_height));
        const size_t imageCount = CreateSwapchainImages(
            vkDev.device, vkDev.table, vkDev.swapchain, vkDev.swapchain_image_format, 
            &vkDev.swapchain_images, &vkDev.swapchain_image_views);
        vkDev.command_buffers.resize(imageCount);

        VK_CHECK(CreateSemaphoreVk(vkDev.device, vkDev.table, &vkDev.semaphore));
        VK_CHECK(CreateSemaphoreVk(vkDev.device, vkDev.table, &vkDev.render_semaphore));

        auto frame_count = vkDev.swapchain_images.size();
        vkDev.image_available_semaphores.resize(frame_count);
        vkDev.render_finish_semaphores.resize(frame_count);
        vkDev.in_flight_fences.resize(frame_count);
        vkDev.images_in_flight.resize(frame_count, VK_NULL_HANDLE);
        for(int i = 0; i < frame_count; ++i) {
            VK_CHECK(CreateSemaphoreVk(vkDev.device, vkDev.table, &vkDev.image_available_semaphores[i]));
            VK_CHECK(CreateSemaphoreVk(vkDev.device, vkDev.table, &vkDev.render_finish_semaphores[i]));
            VK_CHECK(CreateFence(vkDev.device, vkDev.table, &vkDev.in_flight_fences[i]));
        }

        {
            // Create graphics command pool
            VkCommandPoolCreateInfo cpi = {};
            cpi.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
            cpi.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT; //0;
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
        return VK_SUCCESS;
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

    VkResult CreateDepthResources(VulkanRenderDevice& vkDev, uint32_t width, uint32_t height, VulkanImage& depth) {
        VkFormat depthFormat = FindDepthFormat(vkDev.physical_device);

        VK_CHECK(CreateImage(
            vkDev.device, 
            vkDev.physical_device, 
            vkDev.table, 
            width, height, depthFormat, 
            VK_IMAGE_TILING_OPTIMAL, 
            VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, 
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
            depth.image, 
            depth.image_memory));

        VK_CHECK(CreateImageView(
            vkDev.device, 
            vkDev.table, 
            depth.image, 
            depthFormat, 
            VK_IMAGE_ASPECT_DEPTH_BIT, 
            &depth.image_view));

        TransitionImageLayout(
            vkDev, 
            depth.image, 
            depthFormat, 
            VK_IMAGE_LAYOUT_UNDEFINED, 
            VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

        return VK_SUCCESS;
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

    VkResult CreateDescriptorSet(
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

        std::vector<VkDescriptorSetLayout> layouts(
            vkDev.swapchain_images.size(), vkState->descriptor_set_layout
        );

        VkDescriptorSetAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.pNext = nullptr;
        allocInfo.descriptorPool = vkState->descriptor_pool;
        allocInfo.descriptorSetCount = static_cast<uint32_t>(vkDev.swapchain_images.size());
        allocInfo.pSetLayouts = layouts.data();

        vkState->descriptor_sets.resize(vkDev.swapchain_images.size());
        VK_CHECK(vkDev.table.vkAllocateDescriptorSets(vkDev.device, &allocInfo, vkState->descriptor_sets.data()));

        for (size_t i = 0; i < vkDev.swapchain_images.size(); i++) {
            VkDescriptorBufferInfo bufferInfo = {};
            // TODO
            bufferInfo.buffer = nullptr;//vkState->uniform_buffers[i];
            bufferInfo.offset = 0;
            bufferInfo.range = uniformBufferSize;
            VkDescriptorBufferInfo bufferInfo2 = {};
            bufferInfo2.buffer = vkState->storage_buffer;
            bufferInfo2.offset = 0;
            bufferInfo2.range = vertexBufferSize;
            VkDescriptorBufferInfo bufferInfo3 = {};
            bufferInfo3.buffer = vkState->storage_buffer;
            bufferInfo3.offset = vertexBufferSize;
            bufferInfo3.range = indexBufferSize;
            VkDescriptorImageInfo imageInfo = {};
            imageInfo.sampler = vkState->texture_sampler;
            imageInfo.imageView = vkState->texture.image_view;
            imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

            VkWriteDescriptorSet set0 = {};
            set0.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            set0.dstSet = vkState->descriptor_sets[i];
            set0.dstBinding = 0;
            set0.dstArrayElement = 0;
            set0.descriptorCount = 1;
            set0.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            set0.pBufferInfo = &bufferInfo;

            VkWriteDescriptorSet set1 = {};
            set1.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            set1.dstSet = vkState->descriptor_sets[i];
            set1.dstBinding = 1;
            set1.dstArrayElement = 0;
            set1.descriptorCount = 1;
            set1.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
            set1.pBufferInfo = &bufferInfo2;

            VkWriteDescriptorSet set2 = {};
            set2.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            set2.dstSet = vkState->descriptor_sets[i];
            set2.dstBinding = 2;
            set2.dstArrayElement = 0;
            set2.descriptorCount = 1;
            set2.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
            set2.pBufferInfo = &bufferInfo3;

            VkWriteDescriptorSet set3 = {};
            set3.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            set3.dstSet = vkState->descriptor_sets[i];
            set3.dstBinding = 3;
            set3.dstArrayElement = 0;
            set3.descriptorCount = 1;
            set3.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            set3.pImageInfo = &imageInfo;

            const std::array<VkWriteDescriptorSet, 4> descriptorWrites = {
                set0, set1, set2, set3,
            };

            // vkDev.table.vkUpdateDescriptorSets(vkDev.device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
        }
        return VK_SUCCESS;
    }

    VkResult CreateColorAndDepthRenderPass(
        VulkanRenderDevice& vkDev, 
        const bool useDepth, 
        VkRenderPass* renderPass, 
        const VulkanRenderPassCreateInfo& ci, 
        const VkFormat& colorFormat
    ) {
        const bool offscreenInt = ci.flags & eRenderPassBit_OffscreenInternal;
        const bool first = ci.flags & eRenderPassBit_First;
        const bool last  = ci.flags & eRenderPassBit_Last;

        VkAttachmentDescription colorAttachment = {};
        colorAttachment.flags = 0;
        colorAttachment.format = colorFormat;
        colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        auto loadOp = offscreenInt ? VK_ATTACHMENT_LOAD_OP_LOAD : (ci.clear_color ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_LOAD);
        colorAttachment.loadOp = loadOp;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        auto initialLayout = first ? VK_IMAGE_LAYOUT_UNDEFINED : (offscreenInt ? VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL : VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
        colorAttachment.initialLayout = initialLayout;
        auto finalLayout = last ? VK_IMAGE_LAYOUT_PRESENT_SRC_KHR : VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        colorAttachment.finalLayout = finalLayout;

        VkAttachmentDescription depthAttachment = {};
        depthAttachment.flags = 0;
        depthAttachment.format = useDepth ? FindDepthFormat(vkDev.physical_device) : VK_FORMAT_D32_SFLOAT;
        depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        depthAttachment.loadOp = offscreenInt ? VK_ATTACHMENT_LOAD_OP_LOAD : (ci.clear_depth ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_LOAD);
        depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;//VK_ATTACHMENT_STORE_OP_STORE;
        depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.initialLayout = ci.clear_depth ? VK_IMAGE_LAYOUT_UNDEFINED : (offscreenInt ? VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL : VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
        depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkAttachmentReference colorAttachmentRef = {};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkAttachmentReference depthAttachmentRef = {};
        depthAttachmentRef.attachment = 1;
        depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        if (ci.flags & eRenderPassBit_Offscreen)
            colorAttachment.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

        std::vector<VkSubpassDependency> dependencies;

        if (ci.flags & eRenderPassBit_Offscreen) {
            colorAttachment.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            depthAttachment.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

            // Use subpass dependencies for layout transitions
            // TODO : check these flags, maybe wrong
            VkSubpassDependency dependency0 = {};
            dependency0.srcSubpass = VK_SUBPASS_EXTERNAL;
            dependency0.dstSubpass = 0;
            dependency0.srcStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
            dependency0.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            dependency0.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
            dependency0.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            dependency0.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

            VkSubpassDependency dependency1 = {};
            dependency1.srcSubpass = 0;
            dependency1.dstSubpass = VK_SUBPASS_EXTERNAL;
            dependency1.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            dependency1.dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
            dependency1.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            dependency1.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
            dependency1.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

            dependencies.resize(2);
            dependencies[0] = dependency0;
            dependencies[1] = dependency1;
        } else {
            VkSubpassDependency dependency = {};
            dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
            dependency.dstSubpass = 0;
            dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;// | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
            dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;// | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
            dependency.srcAccessMask = 0;
            dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;// | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
            dependency.dependencyFlags = 0;

            dependencies.resize(1);
            dependencies[0] = dependency;
        }

        VkSubpassDescription subpass = {};
        subpass.flags = 0;
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        // subpass.inputAttachmentCount = 0;
        // subpass.pInputAttachments = nullptr;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;
        // subpass.pResolveAttachments = nullptr;
        subpass.pDepthStencilAttachment = useDepth ? &depthAttachmentRef : nullptr;
        // subpass.preserveAttachmentCount = 0;
        // subpass.pPreserveAttachments = nullptr;

        std::array<VkAttachmentDescription, 2> attachments = { colorAttachment, depthAttachment };

        VkRenderPassCreateInfo renderPassInfo = {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        // renderPassInfo.pNext = nullptr;
        // renderPassInfo.flags = 0;
        renderPassInfo.attachmentCount = static_cast<uint32_t>(useDepth ? 2 : 1);
        renderPassInfo.pAttachments = attachments.data();
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpass;
        renderPassInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());
        renderPassInfo.pDependencies = dependencies.data();

        return vkDev.table.vkCreateRenderPass(vkDev.device, &renderPassInfo, nullptr, renderPass);
    }

    VkResult CreateColorOnlyRenderPass(
        VulkanRenderDevice& vkDev, 
        VkRenderPass* renderPass, 
        const VulkanRenderPassCreateInfo& ci, 
        const VkFormat& colorFormat
    ) {
        VulkanRenderPassCreateInfo ci2 = ci;
        ci2.clear_depth = false;
        return CreateColorAndDepthRenderPass(vkDev, false, renderPass, ci2, colorFormat);
    }

    VkResult CreateDepthOnlyRenderPass(
        VulkanRenderDevice& vkDev, 
        VkRenderPass* renderPass, 
        const VulkanRenderPassCreateInfo& ci
    ) {
        VkAttachmentDescription depthAttachment = {};
        depthAttachment.flags = 0;
        depthAttachment.format = FindDepthFormat(vkDev.physical_device);
        depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        depthAttachment.loadOp = ci.clear_depth ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.initialLayout = ci.clear_depth ? VK_IMAGE_LAYOUT_UNDEFINED : (ci.flags & eRenderPassBit_OffscreenInternal ? VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL : VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
        depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkAttachmentReference depthAttachmentRef = {};
        depthAttachmentRef.attachment = 0,
        depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        std::vector<VkSubpassDependency> dependencies;

        if (ci.flags & eRenderPassBit_Offscreen) {
            depthAttachment.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

            /* // Use subpass dependencies for layout transitions
            dependencies.resize(2);
            dependencies[0] = {
                .srcSubpass = VK_SUBPASS_EXTERNAL,
                .dstSubpass = 0,
                .srcStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, // VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, //VK_PIPELINE_STAGE_DEPTH_STENCIL_ATTACHMENT_OUTPUT_BIT,
                .srcAccessMask = VK_ACCESS_SHADER_READ_BIT,
                .dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
                .dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT
            };

            dependencies[1] = {
                .srcSubpass = 0,
                .dstSubpass = VK_SUBPASS_EXTERNAL,
                .srcStageMask = VK_PIPELINE_STAGE_DEPTH_STENCIL_ATTACHMENT_OUTPUT_BIT,
                .dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                .srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
                .dstAccessMask = VK_ACCESS_SHADER_READ_BIT,
                .dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT
            };*/
        }

        VkSubpassDescription subpass = {};
        subpass.flags = 0;
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.inputAttachmentCount = 0;
        subpass.pInputAttachments = nullptr;
        subpass.colorAttachmentCount = 0;
        subpass.pColorAttachments = nullptr;
        subpass.pResolveAttachments = nullptr;
        subpass.pDepthStencilAttachment = &depthAttachmentRef;
        subpass.preserveAttachmentCount = 0;
        subpass.pPreserveAttachments = nullptr;

        VkRenderPassCreateInfo renderPassInfo = {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.pNext = nullptr;
        renderPassInfo.flags = 0;
        renderPassInfo.attachmentCount = 1u;
        renderPassInfo.pAttachments = &depthAttachment;
        renderPassInfo.subpassCount = 1u;
        renderPassInfo.pSubpasses = &subpass;
        renderPassInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());
        renderPassInfo.pDependencies = dependencies.data();

        return vkDev.table.vkCreateRenderPass(vkDev.device, &renderPassInfo, nullptr, renderPass);
    }

    VkResult CreatePipelineLayout(
        const VkDevice& device, 
        const VolkDeviceTable& table, 
        VkDescriptorSetLayout dsLayout, 
        VkPipelineLayout* pipelineLayout
    ) {
        VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.pNext = nullptr;
        pipelineLayoutInfo.flags = 0;
        pipelineLayoutInfo.setLayoutCount = 1;
        pipelineLayoutInfo.pSetLayouts = &dsLayout;
        pipelineLayoutInfo.pushConstantRangeCount = 0;
        pipelineLayoutInfo.pPushConstantRanges = nullptr;
        return table.vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, pipelineLayout);
    }

    VkPipelineShaderStageCreateInfo ShaderStageInfo(
        VkShaderStageFlagBits shaderStage, 
        VulkanShaderModule& module, 
        const char* entryPoint
    ) {
        VkPipelineShaderStageCreateInfo ci = {};
        ci.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        ci.pNext = nullptr;
        ci.flags = 0;
        ci.stage = shaderStage;
        ci.module = module.shader_module;
        ci.pName = entryPoint;
        ci.pSpecializationInfo = nullptr;
        return ci;
    }

    VkResult CreateGraphicsPipeline(
        VulkanRenderDevice& vkDev,
        VkRenderPass renderPass, 
        VkPipelineLayout pipelineLayout,
        const std::vector<std::string>& shaderFiles,
        VkPipeline* pipeline,
        bool useDepth,
        bool useBlending,
        bool dynamicScissorState,
        VkPrimitiveTopology topology,
        int32_t customWidth,
        int32_t customHeight,
        uint32_t numPatchControlPoints
    ) {
        std::vector<VulkanShaderModule> shaderModules;
        std::vector<VkPipelineShaderStageCreateInfo> shaderStages;

        shaderStages.resize(shaderFiles.size());
        shaderModules.resize(shaderFiles.size());

        // TODO : use file system
        std::string root = FindRootDir("_root_dir_");

        for (size_t i = 0 ; i < shaderFiles.size() ; i++) {
            const std::string& file = shaderFiles[i];
            VK_CHECK(CreateShaderModule(vkDev.device, vkDev.table, root, file, &shaderModules[i]));
            VkShaderStageFlagBits stage = ShaderStageToVulkan(ShaderStageFromFileName(file));
            shaderStages[i] = ShaderStageInfo(stage, shaderModules[i], "main");
        }

        VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

        VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
        inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        /* The only difference from createGraphicsPipeline() */
        inputAssembly.topology = topology;
        inputAssembly.primitiveRestartEnable = VK_FALSE;

        VkViewport viewport = {};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(customWidth > 0 ? customWidth : vkDev.framebuffer_width);
        viewport.height = static_cast<float>(customHeight > 0 ? customHeight : vkDev.framebuffer_height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        VkRect2D scissor = {};
        scissor.offset = { 0, 0 };
        scissor.extent = { customWidth > 0 ? customWidth : vkDev.framebuffer_width, customHeight > 0 ? customHeight : vkDev.framebuffer_height };

        VkPipelineViewportStateCreateInfo viewportState = {};
        viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.viewportCount = 1;
        viewportState.pViewports = &viewport;
        viewportState.scissorCount = 1;
        viewportState.pScissors = &scissor;

        VkPipelineRasterizationStateCreateInfo rasterizer = {};
        rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
        rasterizer.cullMode = VK_CULL_MODE_NONE;
        rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
        rasterizer.lineWidth = 1.0f;

        VkPipelineMultisampleStateCreateInfo multisampling = {};
        multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        multisampling.sampleShadingEnable = VK_FALSE;
        multisampling.minSampleShading = 1.0f;

        VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
        colorBlendAttachment.blendEnable = VK_TRUE;
        colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
        colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
        colorBlendAttachment.srcAlphaBlendFactor = useBlending ? VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA : VK_BLEND_FACTOR_ONE;
        colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
        colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
        colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

        VkPipelineColorBlendStateCreateInfo colorBlending = {};
        colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending.logicOpEnable = VK_FALSE;
        colorBlending.logicOp = VK_LOGIC_OP_COPY;
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments = &colorBlendAttachment;
        colorBlending.blendConstants[0] = 0.0f;
        colorBlending.blendConstants[1] = 0.0f;
        colorBlending.blendConstants[2] = 0.0f;
        colorBlending.blendConstants[3] = 0.0f;

        VkPipelineDepthStencilStateCreateInfo depthStencil = {};
        depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        depthStencil.depthTestEnable = static_cast<VkBool32>(useDepth ? VK_TRUE : VK_FALSE);
        depthStencil.depthWriteEnable = static_cast<VkBool32>(useDepth ? VK_TRUE : VK_FALSE);
        depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
        depthStencil.depthBoundsTestEnable = VK_FALSE;
        depthStencil.minDepthBounds = 0.0f;
        depthStencil.maxDepthBounds = 1.0f;

        VkDynamicState dynamicStateElt = VK_DYNAMIC_STATE_SCISSOR;

        VkPipelineDynamicStateCreateInfo dynamicState = {};
        dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicState.pNext = nullptr;
        dynamicState.flags = 0;
        dynamicState.dynamicStateCount = 1;
        dynamicState.pDynamicStates = &dynamicStateElt;

        VkPipelineTessellationStateCreateInfo tessellationState = {};
        tessellationState.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
        tessellationState.pNext = nullptr;
        tessellationState.flags = 0;
        tessellationState.patchControlPoints = numPatchControlPoints;

        VkGraphicsPipelineCreateInfo pipelineInfo = {};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = static_cast<uint32_t>(shaderStages.size());
        pipelineInfo.pStages = shaderStages.data();
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &inputAssembly;
        pipelineInfo.pTessellationState = (topology == VK_PRIMITIVE_TOPOLOGY_PATCH_LIST) ? &tessellationState : nullptr;
        pipelineInfo.pViewportState = &viewportState;
        pipelineInfo.pRasterizationState = &rasterizer;
        pipelineInfo.pMultisampleState = &multisampling;
        pipelineInfo.pDepthStencilState = useDepth ? &depthStencil : nullptr;
        pipelineInfo.pColorBlendState = &colorBlending;
        pipelineInfo.pDynamicState = dynamicScissorState ? &dynamicState : nullptr;
        pipelineInfo.layout = pipelineLayout;
        pipelineInfo.renderPass = renderPass;
        pipelineInfo.subpass = 0;
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
        pipelineInfo.basePipelineIndex = -1;

        VK_CHECK(vkDev.table.vkCreateGraphicsPipelines(vkDev.device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, pipeline));

        for (auto m: shaderModules)
            vkDev.table.vkDestroyShaderModule(vkDev.device, m.shader_module, nullptr);

        return VK_SUCCESS;
    }

    VkResult CreateColorAndDepthFramebuffers(
        VulkanRenderDevice& vkDev, 
        VkRenderPass renderPass, 
        VkImageView depthImageView, 
        std::vector<VkFramebuffer>& swapchainFramebuffers
    ) {
        swapchainFramebuffers.resize(vkDev.swapchain_image_views.size());

        for (size_t i = 0; i < vkDev.swapchain_images.size(); i++) {
            std::array<VkImageView, 2> attachments = {
                vkDev.swapchain_image_views[i],
                depthImageView
            };

            VkFramebufferCreateInfo framebufferInfo = {};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.pNext = nullptr;
            framebufferInfo.flags = 0;
            framebufferInfo.renderPass = renderPass;
            framebufferInfo.attachmentCount = static_cast<uint32_t>((depthImageView == VK_NULL_HANDLE) ? 1 : 2);
            framebufferInfo.pAttachments = attachments.data();
            framebufferInfo.width = vkDev.framebuffer_width;
            framebufferInfo.height = vkDev.framebuffer_height;
            framebufferInfo.layers = 1;

            VK_CHECK(vkDev.table.vkCreateFramebuffer(vkDev.device, &framebufferInfo, nullptr, &swapchainFramebuffers[i]));
        }
        return VK_SUCCESS;
    }

    VkResult CreateColorAndDepthFramebuffer(
        VulkanRenderDevice& vkDev,
        uint32_t width, 
        uint32_t height,
        VkRenderPass renderPass, 
        VkImageView colorImageView, 
        VkImageView depthImageView,
        VkFramebuffer* framebuffer
    ) {
        std::array<VkImageView, 2> attachments = { colorImageView, depthImageView };

        VkFramebufferCreateInfo framebufferInfo = {};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.pNext = nullptr;
        framebufferInfo.flags = 0;
        framebufferInfo.renderPass = renderPass;
        framebufferInfo.attachmentCount = (depthImageView == VK_NULL_HANDLE) ? 1u : 2u;
        framebufferInfo.pAttachments = attachments.data();
        framebufferInfo.width = vkDev.framebuffer_width;
        framebufferInfo.height = vkDev.framebuffer_height;
        framebufferInfo.layers = 1;

        return vkDev.table.vkCreateFramebuffer(vkDev.device, &framebufferInfo, nullptr, framebuffer);
    }

    VkResult CreateDepthOnlyFramebuffer(
        VulkanRenderDevice& vkDev,
        uint32_t width, 
        uint32_t height,
        VkRenderPass renderPass, 
        VkImageView depthImageView,
        VkFramebuffer* framebuffer
    ) {
        VkImageView attachment = depthImageView;

        VkFramebufferCreateInfo framebufferInfo = {};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.pNext = nullptr;
        framebufferInfo.flags = 0;
        framebufferInfo.renderPass = renderPass;
        framebufferInfo.attachmentCount = 1u;
        framebufferInfo.pAttachments = &attachment;
        framebufferInfo.width = vkDev.framebuffer_width;
        framebufferInfo.height = vkDev.framebuffer_height;
        framebufferInfo.layers = 1;

        return vkDev.table.vkCreateFramebuffer(vkDev.device, &framebufferInfo, nullptr, framebuffer);
    }
}
