#pragma once
#include "Interface/IRuntimeModule.h"

#if defined(RK_DESKTOP)
#include "Desktop/Window/WindowDesktop.h"
#endif

namespace Rocket {
    using WindowHandle = void*;

    class WindowManager : implements IRuntimeModule {
        RUNTIME_MODULE_TYPE(WindowManager)
    public:
        virtual ~WindowManager() = default;

        virtual int32_t Initialize() final;
        virtual void Finalize() final;
        virtual void Tick(TimeStep ts) final;

#if defined(RK_DESKTOP)
        [[nodiscard]] void* GetNativeWindow() { return window_.GetWindowHandle(); }
#endif
    private:
        [[maybe_unused]] WindowHandle window_handle_ = nullptr;
#if defined(RK_DESKTOP)
        [[maybe_unused]] Window::WindowDesktop window_;
#endif
// TODO: add more platform
    };

    extern IRuntimeModule* g_WindowManager;
}
