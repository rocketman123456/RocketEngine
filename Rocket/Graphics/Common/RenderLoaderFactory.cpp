#include "Common/RenderLoaderFactory.h"

#if defined(RK_VULKAN)
    #include "Vulkan/VulkanLoader.h"
#endif

namespace Rocket {
    RenderLoaderPtr RenderLoaderFactory::CreateRenderLoader() {
#if defined(RK_VULKAN)
        return std::make_shared<VulkanLoader>();
#else
        return nullptr;
#endif
    }
}
