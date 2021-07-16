#include "Log/Log.h"

#include <iostream>

int main() {
    Rocket::Log::Init();

    RK_INFO(Core, "Hello Rocket Engine");
    RK_INFO(App, "Hello Rocket App");

    return 0;
}
