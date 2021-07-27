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
                throw std::out_of_range("Setting Not Exist");
        }
    private:
        std::unordered_map<std::string, Variant> config_map_;
    };
}
