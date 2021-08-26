#pragma once
#include <string>
#include <cstdint>

namespace Rocket {
    typedef struct WindowInfo {
        std::string name = "";
        int32_t width = 0;
        int32_t height = 0;
        bool resizeable = false;
    } WindowInfo;
}
