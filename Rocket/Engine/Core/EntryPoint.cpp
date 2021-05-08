#include "Core/EntryPoint.h"
#include "Module/Application.h"

using namespace Rocket;

extern Rocket::Application* g_Application;

extern void AllocateModule();
extern void DeallocateModule();

int main(int argc, char **argv) {
    // Init Log
    Rocket::Log::Init();
    
    // Initialize Memory Manager

    // Get Command Line Parameters

    // Allocate Modules
    AllocateModule();

    int result = g_Application->Initialize();
    if(result != 0) {
        RK_ERROR("Application Initialize Failed");
    }

    g_Application->Finalize();

    DeallocateModule();

    return 0;
}
