#include "Common/WindowFactory.h"
#include "Vulkan/VulkanLoader.h"
#include "Vulkan/VulkanDevice.h"

using namespace Rocket;

int main() {
    WindowInfo info;
    info.title = "Rocket";
    info.width = 1280;
    info.height = 720;

    auto window = WindowFactory::CreateWindow(info);
    window->Initialize();

    auto loader = std::make_shared<VulkanLoader>();
    loader->SetWindow(window->GetWindowHandle());
    loader->Initialize();

    auto device = std::make_shared<VulkanDevice>();
    device->SetLoader(loader);
    device->SetSize(window->FramebufferWidth(), window->FramebufferHeight());
    device->Initialize();

    while(window->IsRunning()) {
        window->Tick();
    }

    device->Finalize();
    loader->Finalize();
    window->Finalize();
    return 0;
}
