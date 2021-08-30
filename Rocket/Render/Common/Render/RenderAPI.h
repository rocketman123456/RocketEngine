#pragma once

#include <memory>

namespace Rocket {
    class RenderAPI {
    public:
        virtual ~RenderAPI() = default;
    };

    using RenderAPIPtr = std::unique_ptr<RenderAPI>;
}
