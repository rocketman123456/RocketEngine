#include "Core/Core.h"
#include "Module/Application.h"

Rocket::Application* g_Application; 

void AllocateModule() {
    g_Application = RK_NEW Rocket::Application;
}

void DeallocateModule() {
    RK_DELETE g_Application;
}
