#pragma once
#include "Core/Core.h"

#include <vector>
#include <string>

namespace Rocket {
    std::vector<std::string> Split(const std::string& str, const std::string& pattern);
}