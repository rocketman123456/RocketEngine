#pragma once
#include "Memory/MemoryDefine.h"
#include "Core/Declare.h"

#include <memory>

namespace Rocket {
    class RenderInstance {
    public:
        virtual ~RenderInstance() = default;
    };

    using RenderInstancePtr = std::unique_ptr<RenderInstance>;
}
