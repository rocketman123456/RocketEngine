#pragma once
#include "Window/WindowInfo.h"
#include "Utils/TimeStep.h"

namespace Rocket {
    class WindowInstance {
    public:
        WindowInstance() = default;
        virtual ~WindowInstance() = default;
        virtual int32_t Initialize(const WindowInfo& info) = 0;
        virtual void Finalize() = 0;
        virtual void Tick(TimeStep dt) = 0;
        virtual bool GetShouldClose() = 0;
        // Getter
        inline void* GetWindowHandle() const { return handle_; }
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
