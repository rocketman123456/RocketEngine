#pragma once
#include "Core/Declare.h"

#include <memory>

namespace Rocket {
    class RenderAPI {
    public:
        virtual ~RenderAPI() = default;
    };

    using RenderAPIPtr = std::unique_ptr<RenderAPI>;
}
