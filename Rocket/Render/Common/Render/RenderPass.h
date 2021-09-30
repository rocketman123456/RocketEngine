#pragma once
#include "Core/Declare.h"

#include <memory>

namespace Rocket {
    class RenderPass {
    public:
        virtual ~RenderPass() = default;
    };

    using RenderPassPtr = std::unique_ptr<RenderPass>;
}
