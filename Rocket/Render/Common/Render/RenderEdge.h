#pragma once
#include "Core/Declare.h"
#include "Math/Vectors.h"

#include <memory>

namespace Rocket {
    class RenderTriangle;

    _Interface_ RenderEdge {
    public:
        std::shared_ptr<RenderTriangle> t;
        int32_t i;
    };

    using RenderEdgePtr = std::shared_ptr<RenderEdge>;
}
