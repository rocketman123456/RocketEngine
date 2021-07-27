#pragma once
#include "Utils/Variant.h"

#include <string>
#include <unordered_map>
#include <exception>

namespace Rocket {
    class ConfigSetting {
    public:
        inline void AddSetting(const std::string& name, Variant var) { config_map_[name] = var; }
        inline Variant GetSetting(const std::string& name) {
            auto it = config_map_.find(name);
            if(it != config_map_.end()) 
                return it->second;
            else 
                return Variant();
        }
    private:
        std::unordered_map<std::string, Variant> config_map_;
    };
}
