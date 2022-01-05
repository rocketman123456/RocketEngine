#pragma once
#include "Core/MemoryDefine.h"

#include <string>
#include <cstdint>

namespace Rocket {
    struct WindowInfo {
        int32_t width = 0;
        int32_t height = 0;
        int32_t framebuffer_width = 0;
        int32_t framebuffer_height = 0;
        std::string title = "";
    };

    class WindowContainer {
    public:
        WindowContainer(const WindowInfo& info) : info(info) {}
        virtual ~WindowContainer() = default;
        inline const WindowInfo& Info() const { return info; }
        inline const std::string& Title() const { return info.title; }
        inline int32_t Height() const { return info.height; }
        inline int32_t Width() const { return info.width; }
        inline int32_t FramebufferHeight() { return info.framebuffer_height; }
        inline int32_t FramebufferWidth() { return info.framebuffer_width; }

        virtual void Initialize() = 0;
        virtual void Finalize() = 0;
        virtual void Tick() = 0;
        virtual bool IsRunning() = 0;
        virtual void* GetWindowHandle() = 0;
    protected:
        WindowInfo info;
    };

    CLASS_PTR(WindowContainer);
}
