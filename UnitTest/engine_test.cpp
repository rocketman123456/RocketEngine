#include "Core/Core.h"
#include "Module/Application.h"
#include "Module/WindowManager.h"

namespace Rocket {
    class TestApplication : implements Application {
        RUNTIME_MODULE_TYPE(Application);
    public:
        virtual ~TestApplication() = default;
    };
}

Rocket::IApplication* g_Application;
Rocket::IRuntimeModule* g_WindowManager;

void AllocateModule() {
    // Allocate Application
    g_Application = RK_NEW Rocket::TestApplication;
    // Allocate Modules
    g_WindowManager = RK_NEW Rocket::WindowManager;
    // Insert Modules
    g_Application->AddModule(g_WindowManager);
}

void DeallocateModule() {
    // Deallocate Modules
    RK_DELETE g_WindowManager;
    // Deallocate Application
    RK_DELETE g_Application;
}
