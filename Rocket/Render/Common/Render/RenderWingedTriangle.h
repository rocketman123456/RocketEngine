#pragma once
#include "Core/Declare.h"
#include "Render/RenderWingedEdge.h"

#include <memory>

namespace Rocket {
    class RenderWingedTriangle {
    public:
        RenderWingedEdgePtr e;
    };

    using RenderWingedTrianglePtr = std::shared_ptr<RenderWingedTriangle>;
}
