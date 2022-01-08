#pragma once
#include "Core/MemoryDefine.h"
#include "Resource/ResourceHandle.h"

#include <string>

namespace Rocket {
    class ResourceCache {
    public:
        ResourceHandlePtr GetResource(const std::string& name);
    };

    CLASS_PTR(ResourceCache);
}
