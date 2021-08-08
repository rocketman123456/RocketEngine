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
