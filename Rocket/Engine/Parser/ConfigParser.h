#pragma once
#include "Core/Core.h"

#include <yaml-cpp/yaml.h>
#include <filesystem>
#include <unordered_map>
#include <string>

namespace Rocket {
    class ConfigParser {
    public:
        ConfigParser() = default;
        ~ConfigParser() = default;

        [[nodiscard]] int32_t Initialize();

        template<typename T>
        T GetConfigInfo(const std::string& category, const std::string& name) {
            return config_map_[category][name].as<T>();
        }

        std::string GetRootPath() {
            return root_dir_;
        }

        std::string ToString() const {
			std::string output;
            output = "Config Path : " + root_dir_;
			return output;
		}
    private:
        void FindRootDir();
    private:
        std::filesystem::path root_path_;
        std::string root_dir_;
        std::unordered_map<std::string, YAML::Node> config_map_;
    };

    extern ConfigParser* g_ConfigParser;

    inline std::ostream& operator << (std::ostream &os, const ConfigParser& c) {
		os << c.ToString();
		return os;
	}
}
