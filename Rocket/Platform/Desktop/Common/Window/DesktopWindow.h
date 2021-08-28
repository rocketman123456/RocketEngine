#pragma once
#include "Core/Declare.h"
#include "Window/WindowInstance.h"

namespace Rocket {
    class DesktopWindow : implements WindowInstance {
    public:
        DesktopWindow() = default;
        ~DesktopWindow() = default;

        int32_t Initialize(const WindowInfo& info);
        void Finalize();
    };
}