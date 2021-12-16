#include "Utils/PrintFiles.h"

#include <chrono>

namespace Rocket {
    std::uintmax_t ComputeNativeFileSize(const std::filesystem::path& pathToCheck) {
        if (std::filesystem::exists(pathToCheck) && std::filesystem::is_regular_file(pathToCheck)) {
            auto err = std::error_code{};
            auto filesize = std::filesystem::file_size(pathToCheck, err);
            if (filesize != static_cast<uintmax_t>(-1))
                return filesize;
        }
        return static_cast<uintmax_t>(-1);
    }

    void DisplayNativeFileInfo(const std::filesystem::directory_entry& entry, const std::string& lead, std::filesystem::path& filename) {
        auto file_size = ComputeNativeFileSize(entry);
        //auto ftime = std::filesystem::last_write_time(entry);
        //std::time_t cftime = decltype(ftime)::clock::to_time_t(ftime);
        //std::string time_str = std::asctime(std::localtime(&cftime));
        //if(time_str.at(time_str.length() - 1) == '\n')
        //    time_str = time_str.substr(0, time_str.length() - 1);
        //RK_TRACE(File, "{} {}, {}, time: {}", lead, filename.u8string(), file_size, time_str);
        RK_TRACE(File, "{} {}, {}", lead, filename.u8string(), file_size);
    }

    void DisplayNativeDirTree(const std::filesystem::path& pathToShow, int level) {
        if (std::filesystem::exists(pathToShow) && std::filesystem::is_directory(pathToShow)) {
            auto lead = std::string(level * 3, ' ');
            for (const auto& entry : std::filesystem::directory_iterator(pathToShow)) {
                auto filename = entry.path().filename();
                if (std::filesystem::is_directory(entry.status())) {
                    RK_TRACE(File, "{}[+] {}", lead, filename.u8string());
                    DisplayNativeDirTree(entry, level + 1);
                    RK_TRACE(File, "");
                } else if (std::filesystem::is_regular_file(entry.status())) {
                    DisplayNativeFileInfo(entry, lead, filename);
                } else {
                    RK_TRACE(File, "{}[?] {}", lead, filename.u8string());
                }
            }
        }
    }

    void DisplayNativeDirTree(const std::filesystem::path& pathToShow) {
        DisplayNativeDirTree(pathToShow, 0);
    }
}