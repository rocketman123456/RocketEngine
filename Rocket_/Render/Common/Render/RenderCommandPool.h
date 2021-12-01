#pragma once
#include "Memory/MemoryDefine.h"
#include "Core/Declare.h"

#include <memory>

namespace Rocket {
    class RenderCommandPool {
    public:
        virtual ~RenderCommandPool() = default;
    };

    using RenderCommandPoolPtr = std::unique_ptr<RenderCommandPool>;
}
