#include "Log/Log.h"

#include <iostream>
#include <string>

int main() {
    Rocket::Log::Init();

    RK_INFO(Core, "Hello Rocket Engine {}", std::string("Now"));
    RK_TRACE(App, "Hello Rocket App");
    RK_INFO(App, "Hello Rocket App");
    RK_WARN(App, "Hello Rocket App");
    RK_ERROR(App, "Hello Rocket App");
    RK_CRITICAL(App, "Hello Rocket App");

    return 0;
}
