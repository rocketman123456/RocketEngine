#include "Vulkan/VulkanVariable.h"

#include <volk.h>

namespace Rocket {
#ifdef RK_DEBUG_INFO
    const std::vector<const char*> validation_layers = {
        "VK_LAYER_KHRONOS_validation",
    };
#else
    const std::vector<const char*> validation_layers = {
    };
#endif

#ifdef RK_DEBUG_INFO
    const std::vector<const char*> instance_extensions = {
        VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
        VK_EXT_DEBUG_REPORT_EXTENSION_NAME,
        // for indexed textures
        VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME,
    };
#else
    const std::vector<const char*> instance_extensions = {
        // for indexed textures
        VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME,
// #if defined(RK_MACOS)
//         "VK_KHR_surface"
// #endif
    };
#endif

#if defined(RK_MACOS)
    const std::vector<const char*> device_extensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        "VK_KHR_portability_subset",
    };
#else
    const std::vector<const char*> device_extensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    };
#endif
}
