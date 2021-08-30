#pragma once

#include <memory>

namespace Rocket {
    class RenderDriver {
    public:
        virtual ~RenderDriver() = default;
    };

    using RenderDriverPtr = std::unique_ptr<RenderDriver>;
}
