#pragma once
#include "Core/Declare.h"

#include <memory>

namespace Rocket {
    class RenderGraph {
    public:
        virtual ~RenderGraph() = default;
    };

    using RenderGraphPtr = std::unique_ptr<RenderGraph>;
}
