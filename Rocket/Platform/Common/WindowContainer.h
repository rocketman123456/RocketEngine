#pragma once
#include "Core/MemoryDefine.h"

#include <string>
#include <cstdint>

namespace Rocket {
    struct WindowInfo {
        int32_t width = 0;
        int32_t height = 0;
        std::string title = "";
    };

    class WindowContainer {
    public:
        WindowContainer(const WindowInfo& info) : info(info) {}
        virtual ~WindowContainer() = default;
        const WindowInfo& GetInfo() const { return info; }
        const std::string& GetTitle() const { return info.title; }
        int32_t GetHeight() const { return info.height; }
        int32_t GetWidth() const { return info.width; }

        virtual void Initialize() = 0;
        virtual void Finalize() = 0;
        virtual void Tick() = 0;
        virtual void* GetWindowHandle() = 0;
    private:
        WindowInfo info;
    };

    CLASS_PTR(WindowContainer);
}
