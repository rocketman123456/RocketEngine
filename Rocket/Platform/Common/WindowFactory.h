#pragma once
#include "Common/WindowContainer.h"

namespace Rocket {
    class WindowFactory {
    public:
        static WindowContainerPtr CreatePlatformWindow(const WindowInfo& info);
    };
}
