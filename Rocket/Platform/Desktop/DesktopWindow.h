#pragma once
#include "Common/WindowContainer.h"

#include <GLFW/glfw3.h>

namespace Rocket {
    class DesktopWindow : public WindowContainer {
    public:
        DesktopWindow(const WindowInfo& info) : WindowContainer(info) {}
        virtual ~DesktopWindow() = default;

        void Initialize() final;
        void Finalize() final;
        void Tick() final;

        inline void* GetWindowHandle() final { return window; }

    private:
        GLFWwindow* window = nullptr;
    };
}
