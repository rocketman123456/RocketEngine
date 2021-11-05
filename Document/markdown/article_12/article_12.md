# 12. 引擎配置文件<br>

## 12.1. YAML文件格式简介<br>

## 12.2. 通过yaml-cpp读取分析第一个配置文件<br>
`Config/basic.yaml`
```
app-name: Rocket Engine
resource-type: Normal
```
`yaml_test.cpp`
```
#include "Utils/FindRootDir.h"
#include "FileSystem/FileSystem.h"
#include "Log/Log.h"

#include <iostream>
#include <yaml-cpp/yaml.h>

using namespace Rocket;

int main() {
    Log::Init();

    auto root = Rocket::FindRootDir("_root_dir_");
    auto file = FileSystem::OpenSync(root + "/Config/", "basic.yaml", FileOperateMode::ReadBinary);
    FileBuffer buffer;
    file->ReadAll(buffer);
    printf((const char*)buffer.buffer);
    delete[] buffer.buffer;

    YAML::Node node = YAML::LoadFile(root + "/Config/basic.yaml");
    std::cout << "YAML Node Size: " << node.size() << std::endl;
    std::cout << "app-name Node: " << node["app-name"].as<std::string>() << std::endl;
    std::cout << "resource-type Node: " << node["resource-type"].as<std::string>() << std::endl;

    return 0;
}
```
## 12.3. 通过c++的filesystem模块实现批量配置文件读取<br>
`ConfigLoader.h`
```
#pragma once
#include "FileSystem/FileSystem.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <yaml-cpp/yaml.h>

namespace Rocket {
    class ConfigLoader {
    public:
        explicit ConfigLoader();
        explicit ConfigLoader(const std::string& path);

        template<typename T>
        inline T Get(const std::string& file_name, const std::string& key_name) { return file_contents_[file_name][key_name].as<T>(); }

        inline const std::vector<std::string>& GetFiles() const { return files_; }
        inline const std::unordered_map<std::string, YAML::Node>& GetFileContents() const { return file_contents_; }

    private:
        std::string root_dir_;
        std::string file_path_;
        std::string total_path_;
        std::vector<std::string> files_;
        std::unordered_map<std::string, YAML::Node> file_contents_;
    };
}
```
`ConfigLoader.cpp`
```
#include "Config/ConfigLoader.h"
#include "Utils/FindRootDir.h"
#include "Log/Log.h"

#include <filesystem>

namespace Rocket {
    ConfigLoader::ConfigLoader() {
        root_dir_ = Rocket::FindRootDir("_root_dir_") + "/";
        file_path_ = "Config/";
        total_path_ = root_dir_ + file_path_;
        std::filesystem::directory_iterator dir_list(total_path_);
        for (auto& it:dir_list) {
            std::string name = it.path().filename().string();
            std::string type = name.substr(name.find_last_of('.') + 1);
            RK_TRACE(Core, "Find File: {}, Type: {}", name, type);
            if(type == "yaml") {
                files_.push_back(total_path_ + name);
                YAML::Node node = YAML::LoadFile(total_path_ + name);
                std::string base_name = name.substr(0, name.rfind("."));
                file_contents_[base_name] = node;
            }
        }
    }

    ConfigLoader::ConfigLoader(const std::string& path) {
        root_dir_ = Rocket::FindRootDir("_root_dir_") + "/";
        file_path_ = path;
        total_path_ = root_dir_ + file_path_;
        std::filesystem::directory_iterator dir_list(total_path_);
        for (auto& it:dir_list) {
            std::string name = it.path().filename().string();
            std::string type = name.substr(name.find_last_of('.') + 1);
            RK_TRACE(Core, "Find File: {}, Type: {}", name, type);
            if(type == "yaml") {
                files_.push_back(total_path_ + name);
                YAML::Node node = YAML::LoadFile(total_path_ + name);
                std::string base_name = name.substr(0, name.rfind("."));
                file_contents_[base_name] = node;
            }
        }
    }
}
```
`yaml_test.cpp`
```
    ConfigLoader loader;
    std::string result = loader.Get<std::string>("basic", "app-name");
    std::cout << "app-name From Config Loader : " << node["app-name"].as<std::string>() << std::endl;
```
## 12.4. 未来SQL数据库的接入<br>
