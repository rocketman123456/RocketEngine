#include "Vulkan/VulkanLoader.h"

#include <volk.h>

namespace Rocket {
    int32_t VulkanLoader::LoadDriver() {
        if(volkInitialize() != VK_SUCCESS) {
            RK_GRAPHICS_ERROR("Failed to initialize Vulkan");
            return 1;
        }
        return 0;
    }

    //void VulkanLoader::UnloadDriver() {
    //}
    //int32_t VulkanLoader::LoadDevice() {
    //    return 0;
    //}
    //void VulkanLoader::UnloadDevice() {
    //}
    //int32_t VulkanLoader::LoadEngine() {
    //    return 0;
    //}
    //void VulkanLoader::UnloadEngine() {
    //}
}
