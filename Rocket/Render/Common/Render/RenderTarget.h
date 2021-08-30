#pragma once

#include <memory>

namespace Rocket {
    class RenderTarget {
    public:
        virtual ~RenderTarget() = default;
    };

    using RenderTargetPtr = std::unique_ptr<RenderTarget>;
}
