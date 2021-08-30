#pragma once

#include <memory>

namespace Rocket {
    class RenderSubPass {
    public:
        virtual ~RenderSubPass() = default;
    };

    using RenderSubPassPtr = std::unique_ptr<RenderSubPass>;
}
