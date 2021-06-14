#include "Parser/ConfigParser.h"
#include "Utils/Function.h"

namespace Rocket {
    void ConfigParser::FindRootDir() {
        const uint16_t max_depth_count = 8;
        const String required_name = "root_dir.yaml";
        root_path_ = std::filesystem::current_path();
        bool quit = false;
        
        for(int i = 0; i < max_depth_count; ++i) {
            RK_CORE_TRACE("Current Path: {}", root_path_.string());
            std::filesystem::directory_iterator dir_list(root_path_);
            for (auto& it:dir_list) {
                RK_CORE_TRACE("File Name: {}", it.path().filename().string());
                String name = it.path().filename().string();
                if(name == required_name) {
                    std::filesystem::directory_entry entry(it.path());
                    if(entry.status().type() == std::filesystem::file_type::directory) {
                        quit = true;
                        break;
                    }
                }
            }
            if(quit) {
                break;
            }
            root_path_ = root_path_.parent_path();
        }

        RK_CORE_INFO("Current Path: {}", root_path_.string());
        root_dir_ = root_path_.string();
    }

    int32_t ConfigParser::Initialize() {
        FindRootDir();
        auto config_file_path = root_path_.string() + "/Config";
        RK_CORE_INFO("Current Config Path: {}", config_file_path);
        std::filesystem::directory_iterator dir_list(config_file_path);
        for (auto& it:dir_list) {
            RK_CORE_TRACE("File Name: {}", it.path().filename().string());
            auto split_result = Split(it.path().filename().string(), ".");
            config_map_[split_result[0]] = YAML::LoadFile(it.path().string());
        }
        return 0;
    }
}
