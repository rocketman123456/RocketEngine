#pragma once
#include "Common/WindowContainer.h"

namespace Rocket {
    class WindowFactory {
    public:
        static WindowContainerPtr CreateWindow(const WindowInfo& info);
    };
}
