#pragma once
#include "Log/Log.h"

#include <filesystem>

namespace Rocket {
    void DisplayDirTree(const std::filesystem::path& pathToShow);
}