#pragma once
#include "Core/Core.h"
#include "Core/Template.h"

#include <yaml-cpp/yaml.h>
#include <filesystem>

namespace Rocket {
    class ConfigParser {
    public:
        ConfigParser() = default;
        ~ConfigParser() = default;

        [[nodiscard]] int32_t Initialize();

        template<typename T>
        T GetConfigInfo(const String& category, const String& name) {
            return config_map_[category][name].as<T>();
        }

        String GetRootPath() {
            return root_dir_;
        }

        String ToString() const {
			String output;
            output = "Config Path : " + root_dir_;
			return output;
		}
    private:
        void FindRootDir();
    private:
        std::filesystem::path root_path_;
        String root_dir_;
        UMap<String, YAML::Node> config_map_;
    };

    extern ConfigParser* g_ConfigParser;

    inline std::ostream& operator << (std::ostream &os, const ConfigParser& c) {
		os << c.ToString();
		return os;
	}
}
