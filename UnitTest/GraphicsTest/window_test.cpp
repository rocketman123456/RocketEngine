#include "Common/WindowFactory.h"

using namespace Rocket;

int main() {
    WindowInfo info;
    info.title = "Rocket";
    info.width = 1280;
    info.height = 720;
    auto window = WindowFactory::CreateWindow(info);
    window->Initialize();

    while(window->IsRunning()) {
        window->Tick();
    }

    window->Finalize();
    return 0;
}
