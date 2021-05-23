#include "Core/EntryPoint.h"

namespace Rocket {
    extern IApplication* g_Application;
}

extern void AllocateModule();
extern void DeallocateModule();

using namespace Rocket;

int main(int argc, char **argv) {
    RK_PROFILE_BEGIN_SESSION("Initialize", "Initialize.json");

    // Init Log
    {
        RK_PROFILE_SCOPE("Rocket::Log::Init()");
        Rocket::Log::Init();
    }

    // Initialize Memory Manager
    {
        RK_PROFILE_SCOPE("Initialize Memory Manager");
    }

    // Get Command Line Parameters
    {
        RK_PROFILE_SCOPE("Get Command Line Parameters");
    }

    // Allocate Modules
    {
        RK_PROFILE_SCOPE("AllocateModule()");
        AllocateModule();
    }

    // Initialize Application
    {
        RK_PROFILE_SCOPE("Application::Initialize()");
        int result = g_Application->Initialize();
        if(result != 0) {
            RK_ERROR("Application Initialize Failed");
            return 1;
        }
    }
    RK_PROFILE_END_SESSION();

    RK_PROFILE_BEGIN_SESSION("Tick", "Tick.json");
    // Application Tick [ For Test ]
    for(int i = 0; ; ++i) {
        RK_PROFILE_SCOPE("Application::Tick()");
        g_Application->Tick(1);
    }
    RK_PROFILE_END_SESSION();

    RK_PROFILE_BEGIN_SESSION("Finalize", "Finalize.json");
    // Finalize Application
    {
        RK_PROFILE_SCOPE("Application::Finalize()");
        g_Application->Finalize();
    }
    // Deallocate Modules
    {
        RK_PROFILE_SCOPE("DeallocateModule()");
        DeallocateModule();
    }
    RK_PROFILE_END_SESSION();

    return 0;
}
