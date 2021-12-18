#pragma once
#include <filesystem>
#include <string>

namespace Rocket {
    static bool IsNativeReadOnly(const std::string& real_path) {
        auto perm = std::filesystem::status(real_path).permissions();
        if((perm & std::filesystem::perms::owner_write) == std::filesystem::perms::none) {
            return true;
        }
        return false;
    }
}
