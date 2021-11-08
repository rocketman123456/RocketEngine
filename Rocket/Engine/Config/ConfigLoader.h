#pragma once
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
