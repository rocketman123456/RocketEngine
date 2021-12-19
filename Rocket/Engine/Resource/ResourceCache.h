#pragma once
#include "Core/MemoryDefine.h"

#include <cstdef>

namespace Rocket {
    class ResourceCache {
    public:
        // Capacity in bytes
        ResourceCache(std::size_t capacity);
        ~ResourceCache();
    };
}
