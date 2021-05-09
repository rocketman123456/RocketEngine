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
        WindowManager() : window_handle_(nullptr) {}
        virtual ~WindowManager() = default;

        virtual int32_t Initialize() final;
        virtual void Finalize() final;
        virtual void Tick(TimeStep ts) final;

    private:
        WindowHandle window_handle_;
#if defined(RK_DESKTOP)
        Window::WindowDesktop window_;
#endif
    };

    extern IRuntimeModule* g_WindowManager;
}
