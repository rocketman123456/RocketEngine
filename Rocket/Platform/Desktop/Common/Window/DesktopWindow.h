#pragma once
#include "Window/WindowInfo.h"

namespace Rocket {
    class DesktopWindow {
    public:
        DesktopWindow() = default;
        ~DesktopWindow() = default;

        int32_t Initialize(const WindowInfo& info);
        void Finalize();

        // Cannot Copy Window Class
        DesktopWindow(const DesktopWindow& buffer) = delete;
        DesktopWindow(DesktopWindow&& buffer) = delete;
        DesktopWindow& operator = (const DesktopWindow& buffer) = delete;
        DesktopWindow& operator = (DesktopWindow&& buffer) = delete;
    private:
        WindowInfo info_;
        void* handle_ = nullptr;
    };
}