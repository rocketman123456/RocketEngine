#pragma once
#include "Core/Declare.h"

#include <memory>

namespace Rocket {
    class RenderCommandBuffer {
    public:
        virtual ~RenderCommandBuffer() = default;
    };

    using RenderCommandBufferPtr = std::unique_ptr<RenderCommandBuffer>;
}
