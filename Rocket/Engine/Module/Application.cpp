#include "Module/Application.h"

namespace Rocket {
    int Application::Initialize() {
        PreInitializeModule();
        auto result = InitializeModule();
        if(result != 0) {
            RK_CORE_ERROR("Initialize Module Failed");
            return result;
        }
        PostInitializeModule();

        PreInitialize();
        PostInitialize();

        RK_TRACE("Application::Initialized");
        return 0;
    }

    void Application::Finalize() {
        // Free Game Resources

        FinalizeModule();
        RK_TRACE("Application::Finalized");
    }

    void Application::Tick(TimeStep ts) {
        //RK_TRACE("Application::Tick {}", ts);
        for(auto module : modules_) {
            module->Tick(ts);
        }
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
