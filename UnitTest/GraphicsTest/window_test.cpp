#include "Common/WindowFactory.h"
#include "Common/RenderLoaderFactory.h"

using namespace Rocket;

int main() {
    WindowInfo info;
    info.title = "Rocket";
    info.width = 1280;
    info.height = 720;

    auto window = WindowFactory::CreateWindow(info);
    auto loader = RenderLoaderFactory::CreateRenderLoader();

    window->Initialize();
    loader->Load();

    while(window->IsRunning()) {
        window->Tick();
    }

    loader->Unload();
    window->Finalize();
    return 0;
}
