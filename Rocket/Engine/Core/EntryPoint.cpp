#include "Core/EntryPoint.h"
#include "Module/Application.h"

extern Rocket::IApplication* g_Application;

extern void AllocateModule();
extern void DeallocateModule();

int main(int argc, char* argv[]) {
    // Init Log
    Rocket::Log::Init();
    
    // Initialize Memory Manager

    // Get Command Line Parameters

    // Allocate Modules
    AllocateModule();

    // Initialize Application
    int result = g_Application->Initialize();
    if(result != 0) {
        RK_ERROR("Application Initialize Failed");
    }

    // Application Tick [ For Test ]
    for(int i = 0; i < 10; ++i) {
        g_Application->Tick(1);
    }

    // Finalize Application
    g_Application->Finalize();

    // Deallocate Modules
    DeallocateModule();

    return 0;
}
