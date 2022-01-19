#include "Common/WindowFactory.h"
// #include "Vulkan/VulkanLoader.h"
// #include "Vulkan/VulkanDeviceLoader.h"
#include "Vulkan/VulkanCheck.h"
#include "Vulkan/VulkanConstant.h"
#include "Vulkan/VulkanVariable.h"
#include "Vulkan/VulkanInstanceUtils.h"
#include "Vulkan/VulkanDeviceUtils.h"
#include "Log/Log.h"

#include <GLFW/glfw3.h>

using namespace Rocket;

int main() {
    WindowInfo info;
    info.title = "Rocket";
    info.width = 1600;
    info.height = 900;

    auto window = WindowFactory::CreatePlatformWindow(info);
    window->Initialize();

    VulkanInstance instance;
    instance.window = window->GetWindowHandle();
    BL_CHECK(InitVulkanInstance(instance, instance_extensions, validation_layers));

    VulkanRenderDevice device;
    auto width = window->FramebufferWidth();
    auto height = window->FramebufferHeight();
    BL_CHECK(InitVulkanRenderDevice(instance, device, device_extensions, validation_layers, width, height));

    // auto loader = std::make_shared<VulkanLoader>();
    // loader->SetWindow(window->GetWindowHandle());
    // loader->Initialize();

    // auto device = std::make_shared<VulkanDeviceLoader>();
    // device->SetLoader(loader);
    // device->SetSize(window->FramebufferWidth(), window->FramebufferHeight());
    // device->Initialize();

    while(window->IsRunning()) {
        window->Tick();
    }

    CleanupVulkanRenderDevice(device);
    CleanupVulkanInstance(instance);

    // device->Finalize();
    // loader->Finalize();

    window->Finalize();
    return 0;
}
