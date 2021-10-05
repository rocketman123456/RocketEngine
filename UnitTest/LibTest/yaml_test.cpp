#include "Utils/FindRootDir.h"
#include "FileSystem/FileSystem.h"
#include "Config/ConfigLoader.h"
#include "Log/Log.h"

#include <iostream>
#include <yaml-cpp/yaml.h>

using namespace Rocket;

int main() {
    Log::Init();

    auto root = Rocket::FindRootDir("_root_dir_");
    auto file = FileSystem::OpenSync(root + "/Config/", "basic.yaml", FileOperateMode::READ_BINARY);
    FileBuffer buffer;
    file->ReadAll(buffer);
    printf((const char*)buffer.buffer);

    YAML::Node node = YAML::LoadFile(root + "/Config/basic.yaml");
    std::cout << "YAML Node Size: " << node.size() << std::endl;
    std::cout << "app-name Node: " << node["app-name"].as<std::string>() << std::endl;
    std::cout << "resource-type Node: " << node["resource-type"].as<std::string>() << std::endl;

    ConfigLoader loader;
    std::string result = loader.Get<std::string>("basic", "app-name");
    std::cout << "app-name From Config Loader : " << node["app-name"].as<std::string>() << std::endl;

    Rocket::Log::End();
    return 0;
}
