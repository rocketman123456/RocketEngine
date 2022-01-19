#include "Common/WindowFactory.h"

#if defined(RK_DESKTOP)
    #include "Desktop/DesktopWindow.h"
#elif defined(RK_MOBILE)
    #pragma message("Mobile Platform Not Support Now")
#endif

namespace Rocket {
    WindowContainerPtr WindowFactory::CreatePlatformWindow(const WindowInfo& info) {
#if defined(RK_DESKTOP)
        return std::make_shared<DesktopWindow>(info);
#elif defined(RK_MOBILE)
        return nullptr;
#else
        return nullptr;
#endif
    }
}
