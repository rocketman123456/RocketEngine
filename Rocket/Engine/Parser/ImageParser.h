#pragma once

#include <Eigen/Eigen>

#include <string>
#include <string_view>
#include <vector>

namespace Rocket {
    class ImageParser {
    public:
        ImageParser() = default;
        ImageParser(const std::string& full) : full_path_(full) {}
        ImageParser(const std::string& path, const std::string& name) : path_(path), name_(name), full_path_(path + name) {}
        ~ImageParser() = default;

        inline void Reset(const std::string& full) { full_path_ = full; }
        inline void Reset(const std::string& path, const std::string& name) { path_ = path; name_ = name; full_path_ = path + name; }

        void Parse();
        void Parse(std::vector<Eigen::Vector4f>& data);
        void Parse(std::vector<Eigen::Vector3f>& data);

    private:
        std::string path_;
        std::string name_;
        std::string full_path_;
    };
}
