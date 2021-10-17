#pragma once
#include "Core/Declare.h"

#include <cstdint>
#include <string_view>

namespace Rocket {
    _Interface_ Parser {
    public:
        virtual bool Parse(std::string_view path, std::string_view name) = 0;
        virtual bool Parse(std::string_view full_path) = 0;
    };
}
