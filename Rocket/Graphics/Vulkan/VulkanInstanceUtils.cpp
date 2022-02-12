#include "Vulkan/VulkanInstanceUtils.h"
#include "Log/Log.h"

#include <GLFW/glfw3.h>

#include <set>

static VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT      messageSeverity, 
    VkDebugUtilsMessageTypeFlagsEXT             messageType, 
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, 
    void*                                       pUserData
) {
    if(messageSeverity < VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
        return VK_FALSE;
    } else if(messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
        RK_WARN(Graphics, "validation layer: {}", pCallbackData->pMessage);
        return VK_FALSE;
    } else if(messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
        RK_ERROR(Graphics, "validation layer: {}", pCallbackData->pMessage);
        return VK_FALSE;
    }
}

static VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugReportCallback(
    VkDebugReportFlagsEXT       flags,
    VkDebugReportObjectTypeEXT  objectType,
    uint64_t                    object,
    size_t                      location,
    int32_t                     messageCode,
    const char*                 pLayerPrefix,
    const char*                 pMessage,
    void*                       UserData
) {
    // https://github.com/zeux/niagara/blob/master/src/device.cpp  
    // [ignoring performance warnings]
    // This silences warnings like "For optimal performance image layout 
    // should be VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL instead of GENERAL."
    if(flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT) {
        return VK_FALSE;
    } else if(flags & VK_DEBUG_REPORT_DEBUG_BIT_EXT) {
        return VK_FALSE;
    } else if(flags & VK_DEBUG_REPORT_WARNING_BIT_EXT) {
        RK_WARN(Graphics, "debug callback({}): {}", pLayerPrefix, pMessage);
        return VK_FALSE;
    } else if(flags & VK_DEBUG_REPORT_ERROR_BIT_EXT) {
        RK_ERROR(Graphics, "debug callback({}): {}", pLayerPrefix, pMessage);
        return VK_FALSE;
    } else if(flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT) {
        RK_INFO(Graphics, "debug callback({}): {}", pLayerPrefix, pMessage);
        return VK_FALSE;
    }
}

//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------

namespace Rocket {
    bool CheckValidationLayerSupport(const std::vector<const char*>& validationLayers) {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        std::set<std::string> requiredValidations(validationLayers.begin(), validationLayers.end());
        for (const auto& layer : availableLayers) {
            requiredValidations.erase(layer.layerName);
        }
        return requiredValidations.empty();
    }

    void PrintVulkanVersion() {
        auto version = volkGetInstanceVersion();
        RK_INFO(Graphics ,"Vulkan version {}.{}.{} initialized.",
            VK_VERSION_MAJOR(version),
            VK_VERSION_MINOR(version),
            VK_VERSION_PATCH(version)
        );
    }

    std::vector<const char*> GetRequiredExtensions(bool enable_debug) {
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
        std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
        return extensions;
    }

    void CreateVulkanInstance(
        VkInstance* instance,
        const std::vector<const char*>& validationLayers,
        const std::vector<const char*>& instanceExtension
    ) {
        BL_CHECK(CheckValidationLayerSupport(validationLayers));

        VkApplicationInfo appinfo = {};
        appinfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appinfo.pNext = nullptr;
        appinfo.pApplicationName = "Vulkan";
        appinfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appinfo.pEngineName = "Rocket Engine";
        appinfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appinfo.apiVersion = VK_API_VERSION_1_1;

        auto extensions = GetRequiredExtensions(true);
        std::set<const char*> unique_extensions = {
            extensions.begin(), extensions.end()
        };
        for(const auto& extension : instanceExtension) {
            unique_extensions.insert(extension);
        }
        for (const auto& extension : unique_extensions) {
            RK_INFO(Graphics, "Required Instance Extension: {}", extension);
        }
        std::vector<const char*> extensions_result = {
            unique_extensions.begin(), unique_extensions.end()
        };

        VkInstanceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pNext = nullptr;
        createInfo.flags = 0;
        createInfo.pApplicationInfo = &appinfo;
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
        createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions_result.size());
        createInfo.ppEnabledExtensionNames = extensions_result.data();

        VK_CHECK(vkCreateInstance(&createInfo, nullptr, instance));

        //volkLoadInstance(*instance);
        volkLoadInstanceOnly(*instance);

        PrintVulkanVersion();
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

    void SetupDebugMessenger(
        const VkInstance& instance, VkDebugUtilsMessengerEXT* messenger
    ) {
        VkDebugUtilsMessengerCreateInfoEXT createInfo;
        PopulateDebugMessengerCreateInfo(createInfo);
        VK_CHECK(vkCreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, messenger));
    }

    void SetupDebugReportCallback(
        const VkInstance& instance, VkDebugReportCallbackEXT* reportCallback
    ) {
        VkDebugReportCallbackCreateInfoEXT createInfo = {};
        PopulateDebugReportCreateInfo(createInfo);
        VK_CHECK(vkCreateDebugReportCallbackEXT(instance, &createInfo, nullptr, reportCallback));
    }

//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------

    VkResult InitVulkanInstance(
        VulkanInstance& instance, 
        const std::vector<const char*>& instanceExtensions,
        const std::vector<const char*>& validationLayers
    ) {
        VkResult result = volkInitialize();
        if(result != VK_SUCCESS) {
            RK_ERROR(Graphics, "Unable to load Vulkan");
            return VK_ERROR_INITIALIZATION_FAILED;
        }

        CreateVulkanInstance(&instance.instance, validationLayers, instanceExtensions);
#if defined(RK_DEBUG_INFO)
        // SetupDebugMessenger(instance.instance, &instance.messenger);
        SetupDebugReportCallback(instance.instance, &instance.report_callback);
#endif
        if (glfwCreateWindowSurface(instance.instance, (GLFWwindow*)instance.window, nullptr, &instance.surface)) {
            RK_ERROR(Graphics, "Unable to Create Vulkan Surface");
            return VK_ERROR_INITIALIZATION_FAILED;
        }

        return VK_SUCCESS;
    }

    void CleanupVulkanInstance(VulkanInstance& instance) {
        vkDestroySurfaceKHR(instance.instance, instance.surface, nullptr);
#if defined(RK_DEBUG_INFO)
        vkDestroyDebugReportCallbackEXT(instance.instance, instance.report_callback, nullptr);
        // vkDestroyDebugUtilsMessengerEXT(instance.instance, instance.debug_messenger, nullptr);
#endif
        vkDestroyInstance(instance.instance, nullptr);
    }
}
