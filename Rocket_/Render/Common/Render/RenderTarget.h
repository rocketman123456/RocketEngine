#pragma once
#include "Memory/MemoryDefine.h"
#include "Core/Declare.h"

#include <memory>

namespace Rocket {
    class RenderTarget {
    public:
        virtual ~RenderTarget() = default;
    };

    using RenderTargetPtr = std::unique_ptr<RenderTarget>;
}