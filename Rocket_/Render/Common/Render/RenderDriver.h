#pragma once
#include "Memory/MemoryDefine.h"
#include "Core/Declare.h"

#include <memory>

namespace Rocket {
    class RenderDriver {
    public:
        virtual ~RenderDriver() = default;
    };

    using RenderDriverPtr = std::unique_ptr<RenderDriver>;
}
