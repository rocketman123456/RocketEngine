// #include "Vulkan/VulkanUtils.h"
// // #include "Vulkan/VulkanVariable.h"
// #include "Log/Instrumentor.h"
// #include "Log/Log.h"

// #include <GLFW/glfw3.h>

// #include <exception>
// #include <stdexcept>
// #include <set>

// static VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugCallback(
//         VkDebugUtilsMessageSeverityFlagBitsEXT      messageSeverity, 
//         VkDebugUtilsMessageTypeFlagsEXT             messageType, 
//         const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, 
//         void*                                       pUserData) {
//     if(messageSeverity < VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
//         return VK_FALSE;
//     } else if(messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
//         RK_WARN(Graphics, "validation layer: {}", pCallbackData->pMessage);
//         return VK_FALSE;
//     } else if(messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
//         RK_ERROR(Graphics, "validation layer: {}", pCallbackData->pMessage);
//         return VK_FALSE;
//     }
// }

// static VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugReportCallback(
//         VkDebugReportFlagsEXT       flags,
//         VkDebugReportObjectTypeEXT  objectType,
//         uint64_t                    object,
//         size_t                      location,
//         int32_t                     messageCode,
//         const char*                 pLayerPrefix,
//         const char*                 pMessage,
//         void*                       UserData) {
//     // https://github.com/zeux/niagara/blob/master/src/device.cpp   [ignoring performance warnings]
//     // This silences warnings like "For optimal performance image layout should be VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL instead of GENERAL."
//     if(flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT) {
//         return VK_FALSE;
//     } else if(flags & VK_DEBUG_REPORT_DEBUG_BIT_EXT) {
//         return VK_FALSE;
//     } else if(flags & VK_DEBUG_REPORT_WARNING_BIT_EXT) {
//         RK_WARN(Graphics, "debug callback({}): {}", pLayerPrefix, pMessage);
//         return VK_FALSE;
//     } else if(flags & VK_DEBUG_REPORT_ERROR_BIT_EXT) {
//         RK_ERROR(Graphics, "debug callback({}): {}", pLayerPrefix, pMessage);
//         return VK_FALSE;
//     } else if(flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT) {
//         RK_INFO(Graphics, "debug callback({}): {}", pLayerPrefix, pMessage);
//         return VK_FALSE;
//     }
// }

// namespace Rocket {
//     // Create Instace
//     bool CheckValidationLayerSupport(const std::vector<const char*>& validationLayers);
//     std::vector<const char*> GetRequiredExtensions(bool enable_debug);
//     // Setup Debug Messenger
//     void PopulateDebugMessengerCreateInfo(
//         VkDebugUtilsMessengerCreateInfoEXT& createInfo);
//     void PopulateDebugReportCreateInfo(
//         VkDebugReportCallbackCreateInfoEXT& createInfo);
//     // Pick Physical Device
//     bool IsDeviceSuitable(
//         const VkPhysicalDevice& device, 
//         const VkSurfaceKHR& surface, 
//         const std::vector<const char*>& deviceExtensions);
//     bool CheckDeviceExtensionSupport(
//         const VkPhysicalDevice& device, 
//         const std::vector<const char*>& deviceExtensions);
//     SwapchainSupportDetails QuerySwapchainSupport(
//         const VkPhysicalDevice& device, 
//         const VkSurfaceKHR& surface);
//     // Create Swap Chain
//     VkSurfaceFormatKHR ChooseSwapSurfaceFormat(
//         const std::vector<VkSurfaceFormatKHR>& availableFormats);
//     VkPresentModeKHR ChooseSwapPresentMode(
//         const std::vector<VkPresentModeKHR>& availablePresentModes);
//     uint32_t ChooseSwapImageCount(const VkSurfaceCapabilitiesKHR& capabilities);
//     VkExtent2D ChooseSwapExtent(
//         const VkSurfaceCapabilitiesKHR& capabilities, 
//         uint32_t width, 
//         uint32_t height);
//     // Buffer Related
//     uint32_t FindMemoryType(
//         const VkPhysicalDevice& physicalDevice, 
//         const uint32_t typeFilter, 
//         const VkMemoryPropertyFlags& properties);
// }

// namespace Rocket {
//     VkInstance CreateVulkanInstance(
//             const std::vector<const char*>& validationLayers, 
//             const std::vector<const char*>& instanceExtension) {
//         VkInstance instance;

//         if (!CheckValidationLayerSupport(validationLayers)) {
//             RK_ERROR(Graphics, "validation layers requested, but not available!");
//             throw std::runtime_error("validation layers requested, but not available!");
//         }

//         VkApplicationInfo appInfo{};
//         appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
//         appInfo.pApplicationName = "Rocket Engine App";
//         appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
//         appInfo.pEngineName = "Rocket Engine";
//         appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
//         appInfo.apiVersion = VK_API_VERSION_1_0;

//         bool enable_debug = validationLayers.size() > 0;
//         auto extensions = GetRequiredExtensions(enable_debug);
//         for(auto extension : instanceExtension) {
//             extensions.push_back(extension);
//         }

//         for (const auto& extension : extensions) {
//             RK_INFO(Graphics, "Required Instance Extension: {}", extension);
//         }

//         VkInstanceCreateInfo createInfo{};
//         createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
//         createInfo.pApplicationInfo = &appInfo;
//         createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
//         createInfo.ppEnabledExtensionNames = extensions.data();
//         createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
//         createInfo.ppEnabledLayerNames = validationLayers.data();

//         if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
//             RK_ERROR(Graphics, "failed to create vulkan instance!");
//             throw std::runtime_error("failed to create vulkan instance!");
//         }

//         return instance;
//     }

//     bool CheckValidationLayerSupport(const std::vector<const char*>& validationLayers) {
//         uint32_t layerCount;
//         vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
//         std::vector<VkLayerProperties> availableLayers(layerCount);
//         vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

//         std::set<std::string> requiredValidations(validationLayers.begin(), validationLayers.end());
//         for (const auto& layer : availableLayers) {
//             requiredValidations.erase(layer.layerName);
//         }

//         return requiredValidations.empty();
//     }

//     std::vector<const char*> GetRequiredExtensions(bool enable_debug) {
//         uint32_t glfwExtensionCount = 0;
//         const char** glfwExtensions;
//         glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
//         std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
//         return extensions;
//     }

//     void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
//         createInfo = {};
//         createInfo.sType = 
//             VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
//         createInfo.messageSeverity = 
//             VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | 
//             VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | 
//             VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
//         createInfo.messageType = 
//             VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | 
//             VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | 
//             VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
//         createInfo.pfnUserCallback = VulkanDebugCallback;
//     }

//     void PopulateDebugReportCreateInfo(VkDebugReportCallbackCreateInfoEXT& createInfo) {
//         createInfo = {};
//         createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
//         createInfo.pNext = nullptr;
//         createInfo.flags =
//             VK_DEBUG_REPORT_WARNING_BIT_EXT |
//             VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT |
//             VK_DEBUG_REPORT_ERROR_BIT_EXT |
//             VK_DEBUG_REPORT_DEBUG_BIT_EXT;
//         createInfo.pfnCallback = &VulkanDebugReportCallback;
//         createInfo.pUserData = nullptr;
//     }

//     void PrintVulkanVersion() {
//         auto version = volkGetInstanceVersion();
//         RK_INFO(Graphics ,"Vulkan version {}.{}.{} initialized.",
//             VK_VERSION_MAJOR(version),
//             VK_VERSION_MINOR(version),
//             VK_VERSION_PATCH(version)
//         );
//     }

//     VkDebugUtilsMessengerEXT SetupDebugMessenger(const VkInstance& instance) {
//         VkDebugUtilsMessengerEXT debug_messenger = {};
//         VkDebugUtilsMessengerCreateInfoEXT createInfo;
//         PopulateDebugMessengerCreateInfo(createInfo);
//         if (vkCreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debug_messenger) != 
//                 VK_SUCCESS) {
//             RK_ERROR(Graphics, "failed to set up debug messenger!");
//             throw std::runtime_error("failed to set up debug messenger!");
//         }
//         return debug_messenger;
//     }

//     VkDebugReportCallbackEXT SetupDebugReportCallback(const VkInstance& instance) {
//         VkDebugReportCallbackEXT report_callback;
//         VkDebugReportCallbackCreateInfoEXT createInfo = {};
//         PopulateDebugReportCreateInfo(createInfo);
//         if(vkCreateDebugReportCallbackEXT(instance, &createInfo, nullptr, &report_callback) != VK_SUCCESS) {
//             RK_ERROR(Graphics, "failed to set up debug report callback!");
//             throw std::runtime_error("failed to set up debug report callback!");
//         }
//         return report_callback;
//     }

//     VkPhysicalDevice PickPhysicalDevice(
//             const VkInstance& instance, 
//             const VkSurfaceKHR& surface, 
//             const std::vector<const char*>& deviceExtensions) {
//         VkPhysicalDevice physical_device = VK_NULL_HANDLE;

//         uint32_t deviceCount = 0;
//         vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
//         if(deviceCount == 0) {
//             RK_ERROR(Graphics, "failed to find GPUs with Vulkan support!");
//             throw std::runtime_error("failed to find GPUs with Vulkan support!");
//         }

//         std::vector<VkPhysicalDevice> devices(deviceCount);
//         vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
//         for(const auto& device : devices) {
//             if (IsDeviceSuitable(device, surface, deviceExtensions)) {
//                 physical_device = device;
//                 break;
//             }
//         }

//         if(physical_device == VK_NULL_HANDLE) {
//             RK_ERROR(Graphics, "failed to find a suitable GPU!");
//             throw std::runtime_error("failed to find a suitable GPU!");
//         }

//         return physical_device;
//     }

//     bool IsDeviceSuitable(
//             const VkPhysicalDevice& device, 
//             const VkSurfaceKHR& surface, 
//             const std::vector<const char*>& deviceExtensions) {
//         QueueFamilyIndices indices = FindQueueFamilies(device, surface);

//         bool extensionsSupported = CheckDeviceExtensionSupport(device, deviceExtensions);
//         bool swapChainAdequate = false;
//         if (extensionsSupported) {
//             auto swapChainSupport = QuerySwapchainSupport(device, surface);
//             swapChainAdequate = !swapChainSupport.formats.empty() && 
//                 !swapChainSupport.presentModes.empty();
//         }

//         VkPhysicalDeviceFeatures supportedFeatures;
//         vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

//         return indices.IsComplete() && 
//             extensionsSupported && 
//             swapChainAdequate && 
//             supportedFeatures.samplerAnisotropy;
//     }

//     bool CheckDeviceExtensionSupport(
//             const VkPhysicalDevice& device, 
//             const std::vector<const char*>& deviceExtensions) {
//         uint32_t extensionCount;
//         vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

//         std::vector<VkExtensionProperties> availableExtensions(extensionCount);
//         vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

//         // for (const auto& extension : availableExtensions) {
//         //     RK_INFO(Graphics, "Available Device Extension: {}", extension.extensionName);
//         // }

//         std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

//         for (const auto& extension : requiredExtensions) {
//             RK_INFO(Graphics, "Required Device Extension: {}", extension);
//         }

//         for (const auto& extension : availableExtensions) {
//             requiredExtensions.erase(extension.extensionName);
//         }

//         for (const auto& extension : requiredExtensions) {
//             RK_INFO(Graphics, "Not Available Device Extension: {}", extension);
//         }

//         return requiredExtensions.empty();
//     }

//     VkDevice CreateLogicalDevice(
//             const VkPhysicalDevice& physicalDevice, 
//             const std::vector<const char*>& deviceExtensions, 
//             const std::vector<const char*>& validationLayers,
//             const QueueFamilyIndices& indices) {
//         VkDevice device;

//         std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
//         std::set<uint32_t> uniqueQueueFamilies = {
//             indices.graphics_family.value(),
//             indices.compute_family.value(),
//             indices.present_family.value(),
//         };

//         float queuePriority = 1.0f;
//         for(auto queueFamily : uniqueQueueFamilies) {
//             VkDeviceQueueCreateInfo queueCreateInfo{};
//             queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
//             queueCreateInfo.queueFamilyIndex = queueFamily;
//             queueCreateInfo.queueCount = 1;
//             queueCreateInfo.pQueuePriorities = &queuePriority;
//             queueCreateInfos.push_back(queueCreateInfo);
//         }

//         VkPhysicalDeviceFeatures deviceFeatures{};
//         // deviceFeatures.geometryShader = VK_TRUE;
//         // deviceFeatures.tessellationShader = VK_TRUE;
//         deviceFeatures.samplerAnisotropy = VK_TRUE;

//         VkDeviceCreateInfo createInfo{};
//         createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
//         createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
//         createInfo.pQueueCreateInfos = queueCreateInfos.data();
//         createInfo.pEnabledFeatures = &deviceFeatures;
//         createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
//         createInfo.ppEnabledExtensionNames = deviceExtensions.data();
//         createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
//         createInfo.ppEnabledLayerNames = validationLayers.data();

//         if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
//             RK_ERROR(Graphics, "failed to create logical device!");
//             throw std::runtime_error("failed to create logical device!");
//         }
//         return device;
//     }

//     uint32_t FindQueueFamilies(const VkPhysicalDevice& device, const VkQueueFlags& desiredFlags) {
//         uint32_t familyCount = 0;
//         vkGetPhysicalDeviceQueueFamilyProperties(device, &familyCount, nullptr);
//         std::vector<VkQueueFamilyProperties> families(familyCount);
//         vkGetPhysicalDeviceQueueFamilyProperties(device, &familyCount, families.data());

//         for (uint32_t i = 0; i != families.size(); i++) {
//             if (families[i].queueCount > 0 && families[i].queueFlags & desiredFlags) {
//                 return i;
//             }
//         }
//         return 0;
//     }

//     uint32_t FindPresentFamilies(const VkPhysicalDevice& device, const VkSurfaceKHR& surface) {
//         uint32_t queueFamilyCount = 0;
//         vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
//         std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
//         vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());
//         int i = 0;
//         for (const auto& queueFamily : queueFamilies) {
//             VkBool32 presentSupport = false;
//             vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
//             if (presentSupport) {
//                 return i;
//             }
//             i++;
//         }
//         return 0;
//     }

//     QueueFamilyIndices FindQueueFamilies(
//             const VkPhysicalDevice& device, 
//             const VkSurfaceKHR& surface) {
//         QueueFamilyIndices indices;
//         uint32_t queueFamilyCount = 0;
//         vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
//         std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
//         vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());
//         int i = 0;
//         for (const auto& queueFamily : queueFamilies) {
//             if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
//                 indices.graphics_family = i;
//             }
//             VkBool32 presentSupport = false;
//             vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
//             if (presentSupport) {
//                 indices.present_family = i;
//             }
//             if(queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT) {
//                 indices.compute_family = i;
//             }
//             if (indices.IsComplete()) {
//                 break;
//             }
//             i++;
//         }
//         return indices;
//     }

//     VkSwapchainKHR CreateSwapChain(
//             const VkPhysicalDevice& physicalDevice, 
//             const VkDevice& device,
//             const VolkDeviceTable& table,
//             const VkSurfaceKHR& surface, 
//             const QueueFamilyIndices& indices, 
//             VkFormat* swapChainImageFormat,
//             uint32_t width, 
//             uint32_t height, 
//             bool supportScreenshots) {
//         VkSwapchainKHR swapchain;
//         auto swapchainSupport = QuerySwapchainSupport(physicalDevice, surface);
//         auto surfaceFormat = ChooseSwapSurfaceFormat(swapchainSupport.formats);
//         auto presentMode = ChooseSwapPresentMode(swapchainSupport.presentModes);
//         auto imageCount = ChooseSwapImageCount(swapchainSupport.capabilities);
//         auto graphicsFamily = indices.graphics_family.value();
//         auto extent = ChooseSwapExtent(swapchainSupport.capabilities, width, height);

//         VkSwapchainCreateInfoKHR createInfo = {};
//         createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
//         createInfo.flags = 0;
//         createInfo.surface = surface;
//         createInfo.minImageCount = imageCount;
//         createInfo.imageFormat = surfaceFormat.format;
//         createInfo.imageColorSpace = surfaceFormat.colorSpace;
//         createInfo.imageExtent = extent;
//         createInfo.imageArrayLayers = 1;
//         createInfo.imageUsage = 
//             VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | 
//             VK_IMAGE_USAGE_TRANSFER_DST_BIT | 
//             (supportScreenshots ? VK_IMAGE_USAGE_TRANSFER_SRC_BIT : 0u);
        
//         std::set<uint32_t> queueFamilyIndicesCount = {
//             indices.graphics_family.value(),
//             indices.present_family.value(),
//             indices.compute_family.value()
//         };

//         std::vector<uint32_t> queueFamilyIndices(
//             queueFamilyIndicesCount.begin(), 
//             queueFamilyIndicesCount.end()
//         );

//         if (queueFamilyIndicesCount.size() > 1) {
//             createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
//             createInfo.queueFamilyIndexCount = static_cast<uint32_t>(queueFamilyIndices.size());
//             createInfo.pQueueFamilyIndices = queueFamilyIndices.data();
//         } else {
//             createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
//         }

//         createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
//         createInfo.queueFamilyIndexCount = 1;
//         createInfo.pQueueFamilyIndices = &graphicsFamily;
//         createInfo.preTransform = swapchainSupport.capabilities.currentTransform;
//         createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
//         createInfo.presentMode = presentMode;
//         createInfo.clipped = VK_TRUE;
//         createInfo.oldSwapchain = VK_NULL_HANDLE;

//         if(table.vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapchain) != VK_SUCCESS) {
//             RK_ERROR(Graphics, "failed to create swap chain!");
//             throw std::runtime_error("failed to create swap chain!");
//         }

//         *swapChainImageFormat = surfaceFormat.format;

//         return swapchain;
//     }

//     SwapchainSupportDetails QuerySwapchainSupport(
//             const VkPhysicalDevice& device, 
//             const VkSurfaceKHR& surface) {
//         SwapchainSupportDetails details;
//         vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

//         uint32_t formatCount;
//         vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
//         if (formatCount) {
//             details.formats.resize(formatCount);
//             vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
//         }

//         uint32_t presentModeCount;
//         vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);
//         if (presentModeCount) {
//             details.presentModes.resize(presentModeCount);
//             vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
//         }
//         return details;
//     }

//     VkSurfaceFormatKHR ChooseSwapSurfaceFormat(
//             const std::vector<VkSurfaceFormatKHR>& availableFormats) {
//         return { VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
//     }

//     VkPresentModeKHR ChooseSwapPresentMode(
//             const std::vector<VkPresentModeKHR>& availablePresentModes) {
//         for (const auto mode : availablePresentModes)
//             if (mode == VK_PRESENT_MODE_MAILBOX_KHR)
//                 return mode;
//         // FIFO will always be supported
//         return VK_PRESENT_MODE_FIFO_KHR;
//     }

//     uint32_t ChooseSwapImageCount(const VkSurfaceCapabilitiesKHR& capabilities) {
//         const uint32_t imageCount = capabilities.minImageCount + 1;
//         const bool imageCountExceeded = capabilities.maxImageCount > 0 && 
//             imageCount > capabilities.maxImageCount;
//         return imageCountExceeded ? capabilities.maxImageCount : imageCount;
//     }

//     VkExtent2D ChooseSwapExtent(
//             const VkSurfaceCapabilitiesKHR& capabilities, 
//             uint32_t width, uint32_t height) {
//         if (capabilities.currentExtent.width != UINT32_MAX) {
//             return capabilities.currentExtent;
//         } else {
//             VkExtent2D actualExtent = {
//                 static_cast<uint32_t>(width),
//                 static_cast<uint32_t>(height)
//             };
//             actualExtent.width = std::clamp(
//                 actualExtent.width, 
//                 capabilities.minImageExtent.width, 
//                 capabilities.maxImageExtent.width);
//             actualExtent.height = std::clamp(
//                 actualExtent.height, 
//                 capabilities.minImageExtent.height, 
//                 capabilities.maxImageExtent.height);
//             return actualExtent;
//         }
//     }

//     size_t CreateSwapchainImages(
//             const VkDevice& device, 
//             const VolkDeviceTable& table, 
//             const VkSwapchainKHR& swapchain, 
//             const VkFormat& swapChainImageFormat,
//             std::vector<VkImage>& swapchainImages,
//             std::vector<VkImageView>& swapchainImageViews) {
//         swapchainImages.clear();
//         swapchainImageViews.clear();

//         uint32_t imageCount = 0;
//         if(table.vkGetSwapchainImagesKHR(device, swapchain, &imageCount, nullptr) != VK_SUCCESS) {
//             RK_ERROR(Graphics, "Failed to Get Swapchain Images!");
//             throw std::runtime_error("Failed to Get Swapchain Images!");
//         }

//         swapchainImages.resize(imageCount);
//         swapchainImageViews.resize(imageCount);

//         if(table.vkGetSwapchainImagesKHR(device, swapchain, &imageCount, swapchainImages.data()) != VK_SUCCESS) {
//             RK_ERROR(Graphics, "Failed to Get Swapchain Images!");
//             throw std::runtime_error("Failed to Get Swapchain Images!");
//         }

//         for (unsigned i = 0; i < imageCount; i++) {
//             CreateImageView(
//                 device, table,
//                 swapchainImages[i], 
//                 swapChainImageFormat, 
//                 VK_IMAGE_ASPECT_COLOR_BIT, 
//                 &swapchainImageViews[i]);
//         }

//         return static_cast<size_t>(imageCount);
//     }

//     void CreateImageView(
//             const VkDevice& device, 
//             const VolkDeviceTable& table, 
//             const VkImage& image, 
//             const VkFormat& format, 
//             const VkImageAspectFlags& aspectFlags, 
//             VkImageView* imageView, 
//             const VkImageViewType& viewType,
//             uint32_t layerCount, 
//             uint32_t mipLevels) {
//         VkImageViewCreateInfo viewInfo = {};
        
//         viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
//         viewInfo.pNext = nullptr;
//         viewInfo.flags = 0;
//         viewInfo.image = image;
//         viewInfo.viewType = viewType;
//         viewInfo.format = format;
//         viewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
//         viewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
//         viewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
//         viewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
//         viewInfo.subresourceRange.aspectMask = aspectFlags;
//         viewInfo.subresourceRange.baseMipLevel = 0;
//         viewInfo.subresourceRange.levelCount = mipLevels;
//         viewInfo.subresourceRange.baseArrayLayer = 0;
//         viewInfo.subresourceRange.layerCount = layerCount;

//         if(table.vkCreateImageView(device, &viewInfo, nullptr, imageView) != VK_SUCCESS) {
//             RK_ERROR(Graphics, "Failed to Create Image View!");
//             throw std::runtime_error("Failed to Create Image View!");
//         }
//     }

//     void CreateVulkanSemaphore(
//             const VkDevice& device, 
//             const VolkDeviceTable& table, 
//             VkSemaphore* outSemaphore) {
//         VkSemaphoreCreateInfo createInfo = {};
//         createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
//         if(table.vkCreateSemaphore(device, &createInfo, nullptr, outSemaphore) != VK_SUCCESS) {
//             RK_ERROR(Graphics, "Failed to Create Semaphore!");
//             throw std::runtime_error("Failed to Create Semaphore!");
//         }
//     }

//     void CreateCommandPool(
//             const VkDevice& device, 
//             const VolkDeviceTable& table, 
//             const QueueFamilyIndices& indices, 
//             VkCommandPool* command_pool) {
//         VkCommandPoolCreateInfo createInfo = {};
//         createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
//         createInfo.flags = 0;
//         createInfo.queueFamilyIndex = indices.graphics_family.value();
//         if(table.vkCreateCommandPool(device, &createInfo, nullptr, command_pool) != VK_SUCCESS) {
//             RK_ERROR(Graphics, "Unable to Create Command Pool");
//             throw std::runtime_error("Unable to Create Command Pool");
//         }
//     }

//     void CreateCommandBuffer(
//             const VkDevice& device, 
//             const VolkDeviceTable& table,
//             const VkCommandPool& commandPool,
//             uint32_t imageCount,
//             VkCommandBuffer* command_buffer) {
//         VkCommandBufferAllocateInfo allocateInfo = {};
// 		allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
// 		allocateInfo.pNext = nullptr;
// 		allocateInfo.commandPool = commandPool;
// 		allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
// 		allocateInfo.commandBufferCount = imageCount;
//         if(table.vkAllocateCommandBuffers(device, &allocateInfo, command_buffer) != VK_SUCCESS) {
//             RK_ERROR(Graphics, "Unable to Create Command Pool");
//             throw std::runtime_error("Unable to Create Command Pool");
//         }
//     }

//     size_t AllocateVertexBuffer(
//             const VkDevice& device, 
//             const VolkDeviceTable& table, 
//             const VkPhysicalDevice& physicalDevice,
//             const VkCommandPool& commandPool,
//             const VkQueue& graphicsQueue,
//             VkBuffer* storageBuffer, 
//             VkDeviceMemory* storageBufferMemory, 
//             size_t vertexDataSize, 
//             const void* vertexData, 
//             size_t indexDataSize, 
//             const void* indexData) {
//         VkDeviceSize bufferSize = vertexDataSize + indexDataSize;

//         VkBuffer stagingBuffer;
//         VkDeviceMemory stagingBufferMemory;
//         CreateBuffer(device, table, physicalDevice, bufferSize,
//             VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
//             VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
//             &stagingBuffer, &stagingBufferMemory);

//         void* data;
//         table.vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
//             memcpy(data, vertexData, vertexDataSize);
//             memcpy((unsigned char *)data + vertexDataSize, indexData, indexDataSize);
//         table.vkUnmapMemory(device, stagingBufferMemory);

//         CreateBuffer(device, table, physicalDevice, bufferSize,
//             VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
//             VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, storageBuffer, storageBufferMemory);

//         CopyBuffer(device, table, physicalDevice, 
//             commandPool, graphicsQueue,
//             bufferSize, &stagingBuffer, storageBuffer);

//         table.vkDestroyBuffer(device, stagingBuffer, nullptr);
//         table.vkFreeMemory(device, stagingBufferMemory, nullptr);
//         return bufferSize;
//     }

//     bool CreateBuffer(
//             const VkDevice& device, 
//             const VolkDeviceTable& table, 
//             const VkPhysicalDevice& physicalDevice,
//             const VkDeviceSize& size, 
//             const VkBufferUsageFlags& usage, 
//             const VkMemoryPropertyFlags& properties, 
//             VkBuffer* buffer,
//             VkDeviceMemory* bufferMemory) {
//         VkBufferCreateInfo bufferInfo = {};
//         bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
//         bufferInfo.pNext = nullptr;
//         bufferInfo.flags = 0;
//         bufferInfo.size = size;
//         bufferInfo.usage = usage;
//         bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
//         bufferInfo.queueFamilyIndexCount = 0;
//         bufferInfo.pQueueFamilyIndices = nullptr;
//         if(table.vkCreateBuffer(device, &bufferInfo, nullptr, buffer) != VK_SUCCESS) {
//             RK_ERROR(Graphics, "Unable to Create Buffer");
//             throw std::runtime_error("Unable to Create Buffer");
//         }

//         VkMemoryRequirements memRequirements;
//         table.vkGetBufferMemoryRequirements(device, *buffer, &memRequirements);
//         VkMemoryAllocateInfo allocInfo = {};
//         allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
//         allocInfo.pNext = nullptr;
//         allocInfo.allocationSize = memRequirements.size;
//         allocInfo.memoryTypeIndex = 
//             FindMemoryType(physicalDevice, memRequirements.memoryTypeBits, properties);
//         if(table.vkAllocateMemory(device, &allocInfo, nullptr, bufferMemory) != VK_SUCCESS) {
//             RK_ERROR(Graphics, "Unable to Allocate Buffer");
//             throw std::runtime_error("Unable to Allocate Buffer");
//         }
//         table.vkBindBufferMemory(device, *buffer, *bufferMemory, 0);
//         return true;
//     }

//     uint32_t FindMemoryType(
//             const VkPhysicalDevice& physicalDevice, 
//             const uint32_t typeFilter, 
//             const VkMemoryPropertyFlags& properties) {
//         VkPhysicalDeviceMemoryProperties memProperties;
//         vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);
//         for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
//             bool has_property = (memProperties.memoryTypes[i].propertyFlags & properties) == properties;
//             if ((typeFilter & (1 << i)) && has_property) {
//                 return i;
//             }
//         }
//         return 0xFFFFFFFF;
//     }

//     void CopyBuffer(
//             const VkDevice& device, 
//             const VolkDeviceTable& table, 
//             const VkPhysicalDevice& physicalDevice,
//             const VkCommandPool& commandPool,
//             const VkQueue& graphicsQueue,
//             const VkDeviceSize& size,
//             VkBuffer* srcBuffer, 
//             VkBuffer* dstBuffer) {
//         VkCommandBuffer commandBuffer = BeginSingleTimeCommands(device, table, commandPool);
//         VkBufferCopy copyRegion = {};
//         copyRegion.srcOffset = 0;
//         copyRegion.dstOffset = 0;
//         copyRegion.size = size;
//         table.vkCmdCopyBuffer(commandBuffer, *srcBuffer, *dstBuffer, 1, &copyRegion);
//         EndSingleTimeCommands(device, table, graphicsQueue, commandPool, commandBuffer);
//     }

//     VkCommandBuffer BeginSingleTimeCommands(
//             const VkDevice& device, 
//             const VolkDeviceTable& table,
//             const VkCommandPool& commandPool) {
//         VkCommandBuffer commandBuffer;

//         VkCommandBufferAllocateInfo allocInfo = {};
//         allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
//         allocInfo.pNext = nullptr;
//         allocInfo.commandPool = commandPool;
//         allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
//         allocInfo.commandBufferCount = 1;
//         table.vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);

//         VkCommandBufferBeginInfo beginInfo = {};
//         beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
//         beginInfo.pNext = nullptr;
//         beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
//         beginInfo.pInheritanceInfo = nullptr;
//         table.vkBeginCommandBuffer(commandBuffer, &beginInfo);

//         return commandBuffer;
//     }

//     void EndSingleTimeCommands(
//             const VkDevice& device, 
//             const VolkDeviceTable& table,
//             const VkQueue& graphicsQueue,
//             const VkCommandPool& commandPool,
//             const VkCommandBuffer& commandBuffer) {
//         table.vkEndCommandBuffer(commandBuffer);

//         VkSubmitInfo submitInfo = {};
//         submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
//         submitInfo.pNext = nullptr;
//         submitInfo.waitSemaphoreCount = 0;
//         submitInfo.pWaitSemaphores = nullptr;
//         submitInfo.pWaitDstStageMask = nullptr;
//         submitInfo.commandBufferCount = 1;
//         submitInfo.pCommandBuffers = &commandBuffer;
//         submitInfo.signalSemaphoreCount = 0;
//         submitInfo.pSignalSemaphores = nullptr;

//         table.vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
//         table.vkQueueWaitIdle(graphicsQueue);
//         table.vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
//     }

//     bool CreateSharedBuffer(
//             const VkDevice& device, 
//             const VolkDeviceTable& table, 
//             const VkPhysicalDevice& physicalDevice,
//             const VkDeviceSize size, 
//             const VkBufferUsageFlags usage, 
//             const VkMemoryPropertyFlags properties, 
//             QueueFamilyIndices& indices,
//             VkBuffer* buffer, 
//             VkDeviceMemory* bufferMemory) {
//         if (indices.Multiplicity() < 2)
//             return CreateBuffer(device, table, physicalDevice, 
//                 size, usage, properties, buffer, bufferMemory);

//         VkBufferCreateInfo bufferInfo = {};
//         bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
//         bufferInfo.pNext = nullptr;
//         bufferInfo.flags = 0;
//         bufferInfo.size = size;
//         bufferInfo.usage = usage;
//         bufferInfo.sharingMode = (indices.Multiplicity() > 1) ? VK_SHARING_MODE_CONCURRENT : VK_SHARING_MODE_EXCLUSIVE;
//         bufferInfo.queueFamilyIndexCount = static_cast<uint32_t>(indices.FamilyData().size());
//         bufferInfo.pQueueFamilyIndices = (indices.Multiplicity() > 1) ? indices.FamilyData().data() : nullptr;
//         if(table.vkCreateBuffer(device, &bufferInfo, nullptr, buffer) != VK_SUCCESS) {
//             RK_ERROR(Graphics, "Unable to Create Buffer");
//             throw std::runtime_error("Unable to Create Buffer");
//         }

//         VkMemoryRequirements memRequirements;
//         table.vkGetBufferMemoryRequirements(device, *buffer, &memRequirements);
//         VkMemoryAllocateInfo allocInfo = {};
//         allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
//         allocInfo.pNext = nullptr;
//         allocInfo.allocationSize = memRequirements.size;
//         allocInfo.memoryTypeIndex = 
//             FindMemoryType(physicalDevice, memRequirements.memoryTypeBits, properties);
//         if(table.vkAllocateMemory(device, &allocInfo, nullptr, bufferMemory) != VK_SUCCESS) {
//             RK_ERROR(Graphics, "Unable to Allocate Buffer");
//             throw std::runtime_error("Unable to Allocate Buffer");
//         }
//         table.vkBindBufferMemory(device, *buffer, *bufferMemory, 0);
//         return true;
//     }

//     bool CreateUniformBuffer(
//             const VkDevice& device, 
//             const VolkDeviceTable& table, 
//             const VkPhysicalDevice& physicalDevice,
//             const VkDeviceSize& bufferSize,
//             VkBuffer* buffer, 
//             VkDeviceMemory* bufferMemory) {
//         return CreateBuffer(device, table, physicalDevice, bufferSize,
//             VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
//             VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
//             buffer, bufferMemory);
//     }

//     void UploadBufferData(
//             const VkDevice& device, 
//             const VolkDeviceTable& table, 
//             const VkDeviceMemory& bufferMemory, 
//             VkDeviceSize deviceOffset, 
//             const void* data, 
//             const size_t dataSize) {
//         void* mappedData = nullptr;
//         table.vkMapMemory(device, bufferMemory, deviceOffset, dataSize, 0, &mappedData);
//             memcpy(mappedData, data, dataSize);
//         table.vkUnmapMemory(device, bufferMemory);
//     }

//     void DownloadBufferData(
//             const VkDevice& device, 
//             const VolkDeviceTable& table, 
//             const VkDeviceMemory& bufferMemory, 
//             VkDeviceSize deviceOffset, 
//             void* outData, 
//             const size_t dataSize) {
//         void* mappedData = nullptr;
//         table.vkMapMemory(device, bufferMemory, deviceOffset, dataSize, 0, &mappedData);
//             memcpy(outData, mappedData, dataSize);
//         table.vkUnmapMemory(device, bufferMemory);
//     }
// }
