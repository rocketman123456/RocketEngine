#pragma once
#include "Core/MemoryDefine.h"
#include "Resource/ResourceHandle.h"

#include <cstdef>
#include <string>

namespace Rocket {
    _Interface_ ResourceCache {
    public:
        // Capacity in bytes
        virtual ~ResourceCache();

        virtual ResourceHandlePtr GetResource(const std::string& name) = 0;
    };
}
