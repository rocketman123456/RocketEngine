#pragma once

#include <Eigen/Eigen>

#include <string>
#include <string_view>
#include <vector>
#include <utility>

namespace Rocket {
    class ImageParser {
    public:
        ImageParser() = default;
        ImageParser(const std::string& full) : full_path_(full) {}
        ImageParser(const std::string& path, const std::string& name) : path_(path), name_(name), full_path_(path + name) {}
        ~ImageParser() = default;

        inline void Reset(const std::string& full) { full_path_ = full; }
        inline void Reset(const std::string& path, const std::string& name) { path_ = path; name_ = name; full_path_ = path + name; }

        std::pair<int32_t, int32_t> Parse();
        std::pair<int32_t, int32_t> Parse(std::vector<Eigen::Vector4f>& data);
        std::pair<int32_t, int32_t> Parse(std::vector<Eigen::Vector3f>& data);

    private:
        std::string path_;
        std::string name_;
        std::string full_path_;
    };
}
