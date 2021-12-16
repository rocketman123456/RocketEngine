#pragma once
#include "Log/Log.h"

#include <filesystem>

namespace Rocket {
    void DisplayNativeDirTree(const std::filesystem::path& pathToShow);
}
