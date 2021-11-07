#pragma once
#include "Core/Declare.h"

#include <memory>

namespace Rocket {
    class RenderCommandQueue {
    public:
        virtual ~RenderCommandQueue() = default;
    };

    using RenderCommandQueuePtr = std::unique_ptr<RenderCommandQueue>;
}
