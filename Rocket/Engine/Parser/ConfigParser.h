#pragma once
#include "Core/Core.h"
#include "Core/Template.h"

#include <yaml-cpp/yaml.h>

namespace Rocket {
    class ConfigParser {
    public:
        ConfigParser();
        ~ConfigParser() = default;
    private:
        void FindRootDir();
    private:
        String root_dir_;
    };
}
