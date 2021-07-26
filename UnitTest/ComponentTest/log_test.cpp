#include "Log/Log.h"

#include <iostream>
#include <string>

int main() {
    Rocket::Log::Init();

    RK_INFO(Core, "Hello Rocket Engine {}", std::string("Now"));
    RK_INFO(App, "Hello Rocket App");

    return 0;
}
