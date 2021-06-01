#include "Module/WindowManager.h"

namespace Rocket {
    int WindowManager::Initialize() {
        window_handle_ = nullptr;
#if defined(RK_DESKTOP)
        auto result = window_.Initialize();
        if(result != 0) {
            RK_CORE_ERROR("Window Init Failed");
            return result;
        }
#endif
        return 0;
    }

    void WindowManager::Finalize() {
#if defined(RK_DESKTOP)
        window_.Finalize();
#endif
    }

    void WindowManager::Tick(TimeStep ts) {
#if defined(RK_DESKTOP)
        window_.Tick(ts);
#endif
    }
}
