#pragma once
#include "Memory/MemoryDefine.h"
#include "Core/Declare.h"

#include <memory>

namespace Rocket {
    class RenderSubPass {
    public:
        virtual ~RenderSubPass() = default;
    };

    using RenderSubPassPtr = std::unique_ptr<RenderSubPass>;
}
