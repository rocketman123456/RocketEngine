#include "Core/EntryPoint.h"
#include "Module/Application.h"

extern Rocket::Application* g_Application;

int main(int argc, char **argv) {
    // Get Command Line Parameters

    int result = g_Application->Initialize();
    if(result != 0) {
        RK_ERROR("Application Initialize Failed");
    }

    g_Application->Finalize();

    delete g_Application;

    return 0;
}
