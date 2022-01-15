#include "Vulkan/VulkanConstant.h"

#include <volk.h>

namespace Rocket {
    const std::vector<const char*> validation_layers = {
#ifdef RK_DEBUG_INFO
        "VK_LAYER_KHRONOS_validation",
#endif
    };

    const std::vector<const char*> instance_extensions = {
        "VK_KHR_surface",
#if defined (_WIN32)
        "VK_KHR_win32_surface"
#endif
#if defined (__APPLE__)
        "VK_MVK_macos_surface"
#endif
#if defined (__linux__)
        "VK_KHR_xcb_surface"
#endif
#ifdef RK_DEBUG_INFO
        VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
        VK_EXT_DEBUG_REPORT_EXTENSION_NAME,
#endif
        // for indexed textures
        VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME,
    };

    const std::vector<const char*> device_extensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        VK_KHR_MAINTENANCE3_EXTENSION_NAME,
        VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME,
        // for legacy drivers Vulkan 1.1
        VK_KHR_DRAW_INDIRECT_COUNT_EXTENSION_NAME,
#if defined(RK_MACOS)
        "VK_KHR_portability_subset",
#endif
    };
}
