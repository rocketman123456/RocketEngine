#include "Module/Application.h"

namespace Rocket {
    int Application::Initialize() {
        RK_TRACE("Application::Initialize");
        return 0;
    }

    void Application::Finalize() {
        RK_TRACE("Application::Finalize");
    }

    void Application::Tick(TimeStep ts) {
        
    }

    void Application::PreInitializeModule() {
        RK_TRACE("Application::PreInitializeModule");
    }

    void Application::PostInitializeModule() {
        RK_TRACE("Application::PostInitializeModule");
    }

    void Application::PreInitialize() {
        RK_TRACE("Application::PreInitialize");
    }

    void Application::PostInitialize() {
        RK_TRACE("Application::PostInitialize");
    }
}
