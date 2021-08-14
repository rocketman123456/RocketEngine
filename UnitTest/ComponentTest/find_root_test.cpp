#include "Utils/FindRootDir.h"
#include "Log/Log.h"
#include <iostream>

using namespace Rocket;

int main() {
    Rocket::Log::Init();

    std::string name = "_root_dir_";
    auto root = FindRootDir(name);
    std::cout << root << std::endl;

    Rocket::Log::End();
    return 0;
}