#include "Vulkan/VulkanUtils.h"
#include "Log/Instrumentor.h"
#include "Log/Log.h"

#include <GLFW/glfw3.h>

#include <exception>
#include <stdexcept>
#include <optional>

struct QueueFamilyIndices {
    std::optional<uint32_t> graphics_family;
    bool IsComplete() {
        return graphics_family.has_value();
    }
};

static VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, 
        VkDebugUtilsMessageTypeFlagsEXT messageType, 
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, 
        void* pUserData) {
    RK_WARN(Graphics, "validation layer: {}", pCallbackData->pMessage);
    return VK_FALSE;
}

static VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugReportCallback(
        VkDebugReportFlagsEXT      flags,
        VkDebugReportObjectTypeEXT objectType,
        uint64_t                   object,
        size_t                     location,
        int32_t                    messageCode,
        const char* pLayerPrefix,
        const char* pMessage,
        void* UserData) {
    // https://github.com/zeux/niagara/blob/master/src/device.cpp   [ignoring performance warnings]
    // This silences warnings like "For optimal performance image layout should be VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL instead of GENERAL."
    if (flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT)
        return VK_FALSE;
    RK_WARN(Graphics, "debug callback({}): {}", pLayerPrefix, pMessage);
    return VK_FALSE;
}

namespace Rocket {
    // VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
    //     auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    //     if (func != nullptr) {
    //         return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    //     } else {
    //         return VK_ERROR_EXTENSION_NOT_PRESENT;
    //     }
    // }

    // void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
    //     auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    //     if (func != nullptr) {
    //         func(instance, debugMessenger, pAllocator);
    //     }
    // }

    VkInstance CreateVulkanInstance(const std::vector<const char*>& validationLayers) {
        VkInstance instance;

        if (!CheckValidationLayerSupport(validationLayers)) {
            RK_ERROR(Graphics, "validation layers requested, but not available!");
            throw std::runtime_error("validation layers requested, but not available!");
        }

        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Rocket Engine App";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "Rocket Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        bool enable_debug = validationLayers.size() > 0;
        auto extensions = GetRequiredExtensions(enable_debug);
        createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        createInfo.ppEnabledExtensionNames = extensions.data();

        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();

        // VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
        // PopulateDebugMessengerCreateInfo(debugCreateInfo);
        // createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;

        if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
            RK_ERROR(Graphics, "failed to create vulkan instance!");
            throw std::runtime_error("failed to create vulkan instance!");
        }

        return instance;
    }

    bool CheckValidationLayerSupport(const std::vector<const char*>& validationLayers) {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
        for (const char* layerName : validationLayers) {
            bool layerFound = false;
            for (const auto& layerProperties : availableLayers) {
                if (strcmp(layerName, layerProperties.layerName) == 0) {
                    layerFound = true;
                    break;
                }
            }
            if (!layerFound) {
                RK_WARN(Graphics, "Failed to find {}", layerName);
                return false;
            }
        }
        return true;
    }

    std::vector<const char*> GetRequiredExtensions(bool enable_debug) {
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
        std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
        if(enable_debug) {
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
            extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
            // for indexed textures
            extensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
        }
        return extensions;
    }

    void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
        createInfo = {};
        createInfo.sType = 
            VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = 
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | 
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | 
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType = 
            VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | 
            VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | 
            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = VulkanDebugCallback;
    }

    void PopulateDebugReportCreateInfo(VkDebugReportCallbackCreateInfoEXT& createInfo) {
        createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
        createInfo.pNext = nullptr;
        createInfo.flags =
            VK_DEBUG_REPORT_WARNING_BIT_EXT |
            VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT |
            VK_DEBUG_REPORT_ERROR_BIT_EXT |
            VK_DEBUG_REPORT_DEBUG_BIT_EXT;
        createInfo.pfnCallback = &VulkanDebugReportCallback;
        createInfo.pUserData = nullptr;
    }

    void PrintVulkanVersion() {
        auto version = volkGetInstanceVersion();
        RK_INFO(Graphics ,"Vulkan version {}.{}.{} initialized.",
            VK_VERSION_MAJOR(version),
            VK_VERSION_MINOR(version),
            VK_VERSION_PATCH(version)
        );
    }

    VkDebugUtilsMessengerEXT SetupDebugMessenger(
            VkInstance instance, 
            bool enableValidationLayers) {
        VkDebugUtilsMessengerEXT debug_messenger = {};
        if (!enableValidationLayers) 
            return debug_messenger;

        VkDebugUtilsMessengerCreateInfoEXT createInfo;
        PopulateDebugMessengerCreateInfo(createInfo);
        if (vkCreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debug_messenger) != VK_SUCCESS) {
            RK_ERROR(Graphics, "failed to set up debug messenger!");
            throw std::runtime_error("failed to set up debug messenger!");
        }
        return debug_messenger;
    }

    VkDebugReportCallbackEXT SetupDebugReportCallback(VkInstance instance) {
        VkDebugReportCallbackEXT report_callback;

        VkDebugReportCallbackCreateInfoEXT createInfo = {};
        PopulateDebugReportCreateInfo(createInfo);
        if(vkCreateDebugReportCallbackEXT(instance, &createInfo, nullptr, &report_callback) != VK_SUCCESS) {
            RK_ERROR(Graphics, "failed to set up debug report callback!");
            throw std::runtime_error("failed to set up debug report callback!");
        }
        return report_callback;
    }

    VkPhysicalDevice PickPhysicalDevice(VkInstance instance) {
        VkPhysicalDevice physical_device = VK_NULL_HANDLE;

        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
        if(deviceCount == 0) {
            RK_ERROR(Graphics, "failed to find GPUs with Vulkan support!");
            throw std::runtime_error("failed to find GPUs with Vulkan support!");
        }

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
        for(const auto& device : devices) {
            if (IsDeviceSuitable(device)) {
                physical_device = device;
                break;
            }
        }

        if(physical_device == VK_NULL_HANDLE) {
            RK_ERROR(Graphics, "failed to find a suitable GPU!");
            throw std::runtime_error("failed to find a suitable GPU!");
        }
        return physical_device;
    }

    bool IsDeviceSuitable(VkPhysicalDevice device) {
        QueueFamilyIndices indices = FindQueueFamilies(device);
        return indices.IsComplete();
    }

    QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device) {
        QueueFamilyIndices indices;
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());
        int i = 0;
        for (const auto& queueFamily : queueFamilies) {
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                indices.graphics_family = i;
            }
            if (indices.IsComplete()) {
                break;
            }
            i++;
        }
        return indices;
    }

    VkDevice CreateLogicalDevice(VkPhysicalDevice physicalDevice, const std::vector<const char*>& validationLayers) {
        VkDevice device;

        QueueFamilyIndices indices = FindQueueFamilies(physicalDevice);

        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = indices.graphics_family.value();
        queueCreateInfo.queueCount = 1;

        float queuePriority = 1.0f;
        queueCreateInfo.pQueuePriorities = &queuePriority;

        VkPhysicalDeviceFeatures deviceFeatures{};

        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

        createInfo.pQueueCreateInfos = &queueCreateInfo;
        createInfo.queueCreateInfoCount = 1;

        createInfo.pEnabledFeatures = &deviceFeatures;

        createInfo.enabledExtensionCount = 0;

        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();

        if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
            throw std::runtime_error("failed to create logical device!");
        }

        // TODO : move this 
        // vkGetDeviceQueue(device, indices.graphics_family.value(), 0, &graphicsQueue);
    }
}
