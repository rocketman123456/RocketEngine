#pragma once
#include "Window/WindowInfo.h"

namespace Rocket {
    class WindowInstance {
    public:
        // Cannot Copy Window Class
        WindowInstance(const WindowInstance& buffer) = delete;
        WindowInstance(WindowInstance&& buffer) = delete;
        WindowInstance& operator = (const WindowInstance& buffer) = delete;
        WindowInstance& operator = (WindowInstance&& buffer) = delete;
    protected:
        WindowInfo info_;
        void* handle_ = nullptr;
    };
}
