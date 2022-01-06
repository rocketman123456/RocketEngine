#pragma once
#include "Core/MemoryDefine.h"

#include <string>

namespace Rocket {
    struct ResourceHandle {
        std::string name;
        // Extra Data
    };

    CLASS_PTR(ResourceHandle);
}
