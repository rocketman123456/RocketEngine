#include "Utils/FindRootDir.h"
#include "Log/Log.h"
#include <filesystem>

namespace Rocket {
    std::string FindRootDir(const std::string& root_symbol) {
        constexpr uint16_t max_depth_count = 8;
        std::filesystem::path root_path = std::filesystem::current_path();
        bool quit = false;

        for(int i = 0; i < max_depth_count; ++i) {
            RK_TRACE(Core, "Current Path: {}", root_path.string());
            std::filesystem::directory_iterator dir_list(root_path);
            for (auto& it:dir_list) {
                std::string name = it.path().filename().string();
                if(name == root_symbol) {
                    RK_TRACE(Core, "Find File: {}", it.path().filename().string());
                    quit = true;
                    break;
                    //std::filesystem::directory_entry entry(it.path().root_path());
                    //auto type = entry.status().type();
                    //if(type == std::filesystem::file_type::directory) {
                    //    quit = true;
                    //    break;
                    //}
                }
            }
            if(quit) {
                break;
            }
            root_path = root_path.parent_path();
        }

        std::string root_dir = root_path.string();
        RK_INFO(Core, "Current Path: {}", root_dir);
        return root_dir;
    }
}
