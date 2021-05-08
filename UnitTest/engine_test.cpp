#include "Core/Core.h"
#include "Module/Application.h"

namespace Rocket {
    class TestApplication : implements Application {
        RUNTIME_MODULE_TYPE(Application);
    public:
        virtual ~TestApplication() = default;
    };
}

Rocket::Application* g_Application; 

void AllocateModule() {
    g_Application = RK_NEW Rocket::TestApplication;
}

void DeallocateModule() {
    RK_DELETE g_Application;
}
