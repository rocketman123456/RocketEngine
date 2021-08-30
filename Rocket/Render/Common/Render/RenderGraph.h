#pragma once

#include <memory>

namespace Rocket {
    class RenderGraph {
    public:
        virtual ~RenderGraph() = default;
    };

    using RenderGraphPtr = std::unique_ptr<RenderGraph>;
}
