#include "Vulkan/VulkanVariable.h"

#include <volk.h>

namespace Rocket {
    const std::vector<const char*> validation_layers = {
#ifdef RK_DEBUG_INFO
        "VK_LAYER_KHRONOS_validation",
#endif
    };

    const std::vector<const char*> extensions = {
        VK_KHR_SURFACE_EXTENSION_NAME,
#if defined (RK_WINDOWS)
        "VK_KHR_win32_surface"
#elif defined (RK_MACOS)
        "VK_MVK_macos_surface"
#elif defined (RK_LINUX)
        "VK_KHR_xcb_surface"
#endif
        VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
        VK_EXT_DEBUG_REPORT_EXTENSION_NAME,
        // for indexed textures
        VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME,
    };
}
